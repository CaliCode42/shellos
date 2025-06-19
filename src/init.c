/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 17:34:28 by tcali             #+#    #+#             */
/*   Updated: 2025/06/19 13:46:19 by tcali            ###   ########.fr       */
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
		{
			printf("increase nb_pipes : %d\n", nb_pipes);
			nb_pipes++;
		}
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

void	init_data(t_data *data, char **env)
{
	int	i;

	i = 0;
	data->pid = 0;
	data->envp = env;
	data->file1 = NULL;
	data->file2 = NULL;
	while (data->tokens[i])
		i++;
	data->nb_tokens = i;
	data->token = NULL;
	data->nb_pipes = count_pipes(data);
	data->pipe_fd = safe_malloc(sizeof(int *) * data->nb_pipes);
	data->array_alloc = true;
	create_add_token(data);
}
