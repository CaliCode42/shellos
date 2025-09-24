/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 17:34:28 by tcali             #+#    #+#             */
/*   Updated: 2025/09/25 01:00:03 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// if called with total = true : counts all pipes. 
// if called with total = false : counts pipes until && or || is found.
int	count_pipes(t_data *data, t_token *current, bool total)
{
	int	nb_pipes;

	nb_pipes = 0;
	if (data->nb_tokens <= 1)
		return (0);
	if (total == true)
	{
		while (current)
		{
			if (current->type == PIPE)
				nb_pipes++;
			current = current->next;
		}
		return (nb_pipes);
	}
	while (current)
	{
		if (current->type == AND || current->type == OR)
			return (nb_pipes);
		if (current->type == PIPE)
			nb_pipes++;
		current = current->next;
	}
	return (nb_pipes);
}

void	init_pipes(t_data *data, int n)
{
	int	i;

	i = 0;
	data->pipe_fd = safe_malloc(sizeof(int *) * (n));
	data->array_alloc = true;
	while (i < n)
	{
		data->pipe_fd[i] = safe_malloc(sizeof(int) * 2);
		if (pipe(data->pipe_fd[i]) == -1)
			perror("pipe\n");
		i++;
	}
}

void	init_pids(t_data *data)
{
	int	i;

	i = 0;
	while (i <= data->nb_pipes)
	{
		data->pids[i] = 0;
		i++;
	}
}

void	init_env(t_data *data, char **env)
{
	data->envp = array_dup(env);
	if (data->exported_env)
	{
		free_exported(data->exported_env);
		data->exported_env = NULL;
	}
	env_to_export(data->envp, &data->exported_env);
	sort_export(data->exported_env);
	// data->env_set = true;
}

int	init_data(t_data *data)
{
	data->heredoc = ".heredoc_tmp";
	data->nb_cmds = count_cmds(data->token);
	data->nb_tokens = count_tokens(data->token);
	// print_tokens(data);
	if (check_type(data->token, data) == 0)
		return (0);
	data->nb_pipes = count_pipes(data, data->token, false);
	data->total_pipes = count_pipes(data, data->token, true);
	if (g_exit_status != 0)
	{
		data->last_exit = g_exit_status;
		g_exit_status = 0;
	}
	return (1);
}
