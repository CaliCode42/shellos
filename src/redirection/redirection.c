/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:39:47 by tcali             #+#    #+#             */
/*   Updated: 2025/09/17 12:10:11 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_redirection(t_token *token, t_data *data)
{
	t_token	*current;

	current = token;
	while (current)
	{
		if (is_redirection(current->type))
		data->last_exit = redirect_stream(current, find_prev_cmd(current), data);
		if (data->last_exit != 1)
			return ;
		current = current->next;
	}
}

static int	redirect_input(char *file, t_token *token)
{
	struct stat	sb;
	int			fd;

	fd = -1;
	if (stat(file, &sb) == -1)
	{
		// perror("infile : stat failed\n");
		return (-1);
	}
	if (!S_ISREG(sb.st_mode))
	{
		print_error("Is a directory\n", PRE, token->next);
		return (-1);
	}
	if (!(sb.st_mode & S_IRUSR))
	{
		print_error("permission denied: ", POST, token->next);
		return (-1);
	}
	if (token->type == IN)
		fd = open(file, O_RDONLY);
	if (fd == -1)
		perror("open failed");
	return (fd);
}

static int	redirect_output(char *file, t_token *token)
{
	struct stat	sb;
	int			fd;

	fd = -1;
	if (stat(file, &sb) == 0)
	{
		if (!S_ISREG(sb.st_mode))
		{
			print_error("Is a directory", PRE, token->next);
			return (0);
		}
		if (!(sb.st_mode & S_IWUSR))
		{
			print_error("permission denied: ", POST, token->next);
			return (-1);
		}
	}
	if (token->type == APPEND)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (token->type == OUT)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		perror("open failed");
	return (fd);
}

int	redirect_stream(t_token *token, t_token *cmd, t_data *data)
{
	if (!token->next || !token->next->str)
	{
		if (syntax_error(token->str) == 2)
			return (2);
		return (2);
	}
	token->next->type = ARG;
	if (token->type == IN)
	{
		cmd->input = redirect_input(token->next->str, token);
		if (cmd->input == -1)
		{
			print_error("No such file or directory", PRE, token->next);
			return (127);
		}
	}
	else if (token->type == OUT || token->type == APPEND)
	{
		cmd->output = redirect_output(token->next->str, token);
		if (cmd->output == -1)
		{
			print_error("No such file or directory", PRE, token->next);
			return (127);
		}
	}
	else if (token->type == HEREDOC && check_heredoc(token, cmd, data) == 2)
		return (2);
	return (1);
}
// 
// void	reset_stream(t_data *data)
// {
// 	data->input = -1;
// 	data->output = -1;
// }
