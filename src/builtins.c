/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:41:32 by tcali             #+#    #+#             */
/*   Updated: 2025/06/18 15:28:07 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	return (!ft_strncmp(cmd, "echo", 5)
		|| !ft_strncmp(cmd, "cd", 3)
		|| !ft_strncmp(cmd, "pwd", 4)
		|| !ft_strncmp(cmd, "export", 7)
		|| !ft_strncmp(cmd, "unset", 6)
		|| !ft_strncmp(cmd, "env", 4)
		|| !ft_strncmp(cmd, "exit", 5));
}

int	builtin_echo(t_token *token)
{
	int		newline;
	t_token	*current;

	newline = 1;
	current = token->next;
	if (current && !ft_strncmp(current->str, "-n", 3))
	{
		newline = 0;
		current = current->next;
	}
	while (current)
	{
		printf("%s", current->str);
		if (current->next && current->next->str)
			printf(" ");
		current = current->next;
	}
	if (newline)
		printf("\n");
	return (0);
}

int	builtin_exit(t_data *data)
{
	free_minishell(data);
	printf("exit\n");
	exit(EXIT_SUCCESS);
	return (0);
}

int	builtin_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd\n");
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}

/*********************************************************************/
/*
	define next token as an arg.
	if arg == ".." >> close current directory and get back to previous
	if arg is a valid path and access is authorized, go to directory
	if no arg given, and HOME define go to HOME
*/
/*********************************************************************/
int	builtin_cd(t_token *token, t_data *data)
{
	char	*path;

	(void)data;
	if (!token || !token->next || !token->next->str)
	{
		path = getenv("HOME");
		if (!path)
			return (perror("cd : HOME not set\n"), 1);
	}
	else
		path = token->next->str;
	if (!is_valid_dir(path))
		return (1);
	go_to(path);
	return (0);
}
