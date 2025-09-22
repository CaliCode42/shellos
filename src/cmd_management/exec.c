/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 17:30:52 by tcali             #+#    #+#             */
/*   Updated: 2025/09/22 10:28:21 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	split_cmd(char *command, char **env, char ***args, char **path)
{
	*args = ft_split(command, ' ');
	if (!*args || !*args[0])
	{
		free_array(args);
		error_exit("command not found\n");
	}
	*path = get_cmd_path((*args)[0], env);
	if (!*path)
	{
		ft_printf_fd(2, "%s : command not found\n", (*args)[0]);
		free_array(args);
		return (-1);
	}
	return (0);
}

// static char	*exec_bin(char *command, char **args)
// {
// 	args = malloc(sizeof(char *) * 4);
// 	if (!args)
// 		error_exit("malloc failed");
// 	args[0] = ft_strdup("/bin/sh");
// 	args[1] = ft_strdup("-c");
// 	args[2] = ft_strdup(command);
// 	args[3] = NULL;
// 	return (ft_strdup("/bin/sh"));
// }

int	exec_builtin(t_token *token, t_data *data)
{
	if (!ft_strncmp(token->str, "echo", 5))
		return (builtin_echo(data, token));
	if (!ft_strncmp(token->str, "cd", 3))
	{
		// printf("exit code = %d\n", data->last_exit);
		return (builtin_cd(token, data));
	}
	if (!ft_strncmp(token->str, "pwd", 4))
		return (builtin_pwd(token));
	if (!ft_strncmp(token->str, "exit", 5))
		return (builtin_exit(data));
	if (!ft_strncmp(token->str, "env", 4))
		return (builtin_env(data));
	if (!ft_strncmp(token->str, "export", 7))
		return (builtin_export(data, token));
	if (!ft_strncmp(token->str, "unset", 6))
		return (builtin_unset(data, token));
	return (1);
}

void	exec_failed(char **path, char ***args, t_data *data)
{
	if (ft_strchr(*args[0], '/'))
	{
		safe_print(ft_strjoin(*args[0], " : Is a directory."), true, true);
		ft_free((void **)&path);
		free_array(args);
		free_minishell(data, true);
		data->last_exit = 126;
		g_exit_status = 126;
		exit(data->last_exit);
	}
	print_error("command not found", OFF, NULL);
	ft_free((void **)&path);
	free_array(args);
	free_minishell(data, true);
	data->last_exit = 127;
	g_exit_status = 127;
	exit(data->last_exit);
}

// Actually the name speaks for itself, this function executes a command LOL
void	execute_command(char *command, char **env, t_token *token, t_data *data)
{
	char	**args;
	char	*path;

	path = NULL;
	args = NULL;
	if (is_builtin(command))
	{
		if (exec_builtin(token, data) == 0)
		{
			data->last_exit = 1;
			free_minishell(data, true);
			exit(EXIT_SUCCESS);
		}
		free_minishell(data, true);
		exit(127);
	}
	path = get_cmd_path(command, env);
	args = safe_malloc(sizeof(char *) * 2);
	args[0] = ft_strdup(command);
	args[1] = NULL;
	if (!path)
	{
		ft_printf_fd(2, "%s : command not found\n", command);
		exit (127);
	}
	// {
	// 	if (split_cmd(command, env, &args, &path) == -1)
	// 	{
	// 		data->last_exit = 127;
	// 		free_minishell(data, true);
	// 		// printf("data->last_exit = %d\n", data->last_exit);
	// 		exit(data->last_exit);
	// 	}
	// }
	printf("comand = %s\n", command);
	printf("path = %s\n", path);
	if (execve(path, &args[0], env) == -1)
		exec_failed(&path, &args, data);
}
