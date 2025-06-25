/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 17:34:28 by tcali             #+#    #+#             */
/*   Updated: 2025/06/25 17:15:22 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_pipes(t_data *data)
{
	int	nb_pipes;
	int	i;

	nb_pipes = 0;
	i = 0;
	if (data->nb_tokens <= 1)
		return (0);
	while (data->tokens[i])
	{
		if (!ft_strncmp(data->tokens[i], "|", 2))
			nb_pipes++;
		i++;
	}
	return (nb_pipes);
}

void	init_pipes(int **pipes, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pipes[i] = safe_malloc(sizeof(int) * 2);
		if (pipe(pipes[i]) == -1)
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

void	init_data(t_data *data, char **env)
{
	int	i;

	i = 0;
	data->pids = NULL;
	data->input = -1;
	data->output = -1;
	data->cmds = NULL;
	data->token = NULL;
	data->pipe_fd = NULL;
	while (data->tokens[i])
		i++;
	data->nb_tokens = i;
	data->envp = env;
	data->nb_pipes = count_pipes(data);
	if (data->nb_pipes)
	{
		data->pipe_fd = safe_malloc(sizeof(int *) * data->nb_pipes);
		init_pipes(data->pipe_fd, data->nb_pipes);
		data->array_alloc = true;
	}
	data->last_exit = 1;
	create_add_token(data);
}
