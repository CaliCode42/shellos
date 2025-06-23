/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:39:47 by tcali             #+#    #+#             */
/*   Updated: 2025/06/23 21:37:56 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirection(t_tkn_code type)
{
	return (type == IN || type == OUT);
}

static int	redirect_input(char *file)
{
	struct stat	sb;
	int			fd;

	if (stat(file, &sb) == -1)
	{
		perror("infile : stat failed\n");
		return (-1);
	}
	if (!S_ISREG(sb.st_mode))
	{
		printf("infile: %s is not a regular file\n", file);
		return (-1);
	}
	if (!(sb.st_mode & S_IRUSR))
	{
		printf("infile: permission denied: %s\n", file);
		return (-1);
	}
	fd = open(file, O_RDONLY);
	if (fd == -1)
		perror("infile: open failed");
	return (fd);
}

static int	redirect_output(char *file)
{
	struct stat	sb;
	int			fd;

	if (stat(file, &sb) == 0)
	{
		if (!S_ISREG(sb.st_mode))
		{
			printf("outfile: %s is not a regular file\n", file);
			return (-1);
		}
		if (!(sb.st_mode & S_IWUSR))
		{
			printf("outfile: permission denied: %s\n", file);
			return (-1);
		}
	}
	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		perror("infile: open failed");
	return (fd);
}

void	redirect_stream(t_token *token, t_data *data)
{
	if (!token->next || !token->next->str)
	{
		error_exit("bash: syntax error near unexpected token `newline'");
	}
	if (token->type == IN)
	{
		data->input = redirect_input(token->next->str);
		if (data->input == -1)
		{
			perror("input redirection failed\n");
			return ;
		}
	}
	else if (token->type == OUT)
	{
		data->output = redirect_output(token->next->str);
		if (data->output == -1)
		{
			perror("output redirection failed\n");
			return ;
		}
	}
}
