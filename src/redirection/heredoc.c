/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:15:40 by tcali             #+#    #+#             */
/*   Updated: 2025/09/16 13:30:33 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc_present(t_token *token)
{
	t_token	*current;

	current = token;
	while (current)
	{
		if (current->type == HEREDOC)
			return (1);
		current = current->next;
	}
	return (0);
}

void	free_heredoc(t_data *data)
{
	if (!heredoc_present(data->token))
		return ;
	else
	{
		ft_free((void **)&data->delimiter);
		unlink(".heredoc_tmp");
	}
}

// writing on tmp_fd until delimiter is reached.
int	write_heredoc(char *delimiter, int tmp_fd)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || !ft_strncmp(line, delimiter, ft_strlen(delimiter)))
		{
			ft_free((void **)&line);
			break ;
		}
		ft_printf_fd(tmp_fd, "%s\n", line);
		ft_free((void **)&line);
	}
	return (0);
}

// creates a tmp_file to write on.
int	handle_heredoc(t_data *data, char *delimiter)
{
	int		tmp_fd;

	tmp_fd = open(data->heredoc, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmp_fd == -1)
	{
		perror("open HEREDOC");
		return (-1);
	}
	if (write_heredoc(delimiter, tmp_fd) == -1)
	{
		close (tmp_fd);
		return (-1);
	}
	close(tmp_fd);
	tmp_fd = open(data->heredoc, O_RDONLY);
	if (tmp_fd == -1)
	{
		perror("open");
		return (-1);
	}
	return (tmp_fd);
}

// checks for syntax errors and defines the delimiter.
int	check_heredoc(t_token *token, t_token *cmd, t_data *data)
{
	if (!cmd)
	{
		if (!token->next || !token->next->str)
			return (2);
		token->next->type = ARG;
		data->delimiter = ft_strdup(token->next->str);
		token->input = handle_heredoc(data, data->delimiter);
		return (1);
	}
	if (cmd->input != -1)
		close(cmd->input);
	if (!token->next || !token->next->str)
		return (2);
	token->next->type = ARG;
	data->delimiter = ft_strdup(token->next->str);
	cmd->input = handle_heredoc(data, data->delimiter);
	return (1);
}
