/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 17:30:52 by tcali             #+#    #+#             */
/*   Updated: 2025/06/23 16:25:41 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	split_cmd(char *command, char **env, char ***args, char **path)
{
	*args = ft_split(command, ' ');
	if (!*args || !*args[0])
	{
		free_array(*args, 0);
		error_exit("command not found\n");
	}
	*path = get_cmd_path((*args)[0], env);
	if (!*path)
	{
		ft_printf_fd(2, "%s : command not found\n", (*args)[0]);
		free_array(*args, 0);
		return (-1);
	}
	return (0);
}

static char	*exec_bin(char *command, char **args)
{
	args = malloc(sizeof(char *) * 4);
	if (!args)
		error_exit("malloc failed");
	args[0] = ft_strdup("/bin/sh");
	args[1] = ft_strdup("-c");
	args[2] = ft_strdup(command);
	args[3] = NULL;
	return (ft_strdup("/bin/sh"));
}

int	exec_builtin(t_token *token, t_data *data)
{
	if (!ft_strncmp(token->str, "echo", 5))
		return (builtin_echo(token));
	if (!ft_strncmp(token->str, "cd", 3))
		return (builtin_cd(token, data));
	if (!ft_strncmp(token->str, "pwd", 4))
		return (builtin_pwd());
	if (!ft_strncmp(token->str, "exit", 5))
		return (builtin_exit(data));
	return (1);
}

void	execute_command(char *command, char **env)
{
	char	**args;
	char	*path;

	if (ft_strchr(command, ';') || ft_strchr(command, '&')
		|| ft_strchr(command, '|') || ft_strchr(command, '*'))
	{
		path = exec_bin(command, args);
		return ;
	}
	else
	{
		if (split_cmd(command, env, &args, &path) == -1)
			return ;
	}
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	if (execve(path, args, env) == -1)
	{
		perror("execve\n");
		free(path);
		free_array(args, 0);
		printf("error executing command\n");
		exit(EXIT_FAILURE);
	}
}
