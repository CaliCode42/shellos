/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 11:35:11 by tcali             #+#    #+#             */
/*   Updated: 2025/06/23 21:36:22 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_pipes(t_data *data, int i)
{
	if (i > 0)
	{
		if (data->input == -1)
			dup2(data->pipe_fd[i - 1][data->input], STDIN_FILENO);
	}
	if (i < data->nb_pipes)
	{
		if (data->output == -1)
			dup2(data->pipe_fd[i][1], STDOUT_FILENO);
	}
	close_pipes(data->pipe_fd, data->nb_pipes, data);
	execute_command(data->cmds[i], data->envp);
	free_pids(data);
	exit(EXIT_FAILURE);
}

void	child(t_data *data, int i)
{
	if (data->input != -1)
	{
		dup2(data->input, STDIN_FILENO);
		close(data->input);
	}
	if (data->output != -1)
	{
		dup2(data->output, STDOUT_FILENO);
		close(data->output);
	}
	if (!data->nb_pipes)
	{
		execute_command(data->token->str, data->envp);
		free_pids(data);
		exit(EXIT_FAILURE);
	}
	else
		handle_pipes(data, i);
}

void	parent(t_data *data, int i)
{
	int	status;
	int	sig;

	status = 0;
	if (data->nb_pipes && i == data->nb_pipes)
	{
		close_pipes(data->pipe_fd, data->nb_pipes, data);
	}
	wait_all(data, &status);
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
		{
			free_pids(data);
			printf("Quit (Core dumped)\n");
		}
		else if (sig == SIGINT)
		{
			free_pids(data);
			printf("\n");
		}
	}
}

void	create_child(t_data *data)
{
	int	i;

	i = 0;
	data->pids = safe_malloc(sizeof(pid_t) * (data->nb_pipes + 1));
	init_pids(data);
	while (i <= data->nb_pipes)
	{
		data->pids[i] = fork();
		if (data->pids[i] == 0)
			child(data, i);
		else if (data->pids[i] > 0)
			parent(data, i);
		else
		{
			perror("fork");
			break ;
		}
		i++;
	}
	if (data->array_alloc == true)
	{
		free_array(data->cmds, 0);
		data->array_alloc = false;
	}
	free_pids(data);
}

void	fork_process(t_data *data)
{
	t_token	*current;

	current = data->token;
	while (current)
	{
		if (current->type == CMD)
		{
			if (is_builtin(current->str))
				exec_builtin(current, data);
			else
			{
				if (data->nb_pipes)
				{
					token_to_array(data->token, data, data->nb_pipes);
					create_child(data);
					free_pipes(data->pipe_fd, data->nb_pipes, data);
					return ;
				}
				create_child(data);
			}
		}
		current = current->next;
	}
}
