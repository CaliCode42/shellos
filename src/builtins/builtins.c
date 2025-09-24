/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:41:32 by tcali             #+#    #+#             */
/*   Updated: 2025/09/24 20:28:36 by tcali            ###   ########.fr       */
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

int	builtin_echo(t_data *data, t_token *token)
{
	int		newline;
	t_token	*current;
	// char	*tmp;

	(void)data;
	newline = 1;
	// tmp = NULL;
	current = token->next;
	if (current && !ft_strncmp(current->str, "-n", 3))
	{
		newline = 0;
		current = current->next;
	}
	while (current && current->type == ARG)
	{
		// if (current->to_expand == true)
		// {
		// 	tmp = expand_quotes(current->str, data, current);
		// 	ft_free((void **)&current->str);
		// 	current->str = tmp;
		// }
		if (current->str)
		{
			safe_print(current->str, false, false);
			if (current->next && current->next->str)
				safe_print(" ", false, false);
		}
		current = current->next;
	}
	if (newline)
		safe_print("\n", false, false);
	return (0);
}

int	builtin_exit(t_data *data)
{
	safe_print("exit", false, true);
	free_minishell(data, true);
	exit(EXIT_SUCCESS);
	return (0);
}

int	builtin_pwd(t_token *token)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd\n");
		return (1);
	}
	if (token->output != -1)
		ft_printf_fd(token->output, "%s\n", cwd);
	else
		safe_print(cwd, false, true);
	ft_free((void **)&cwd);
	return (0);
}
