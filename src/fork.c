/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 11:35:11 by tcali             #+#    #+#             */
/*   Updated: 2025/06/25 17:37:05 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_pipes(t_data *data, int i, t_token *token)
{
	if (i > 0 && i - 1 < data->nb_pipes)
	{
		if (data->input == -1)
			dup2(data->pipe_fd[i - 1][0], STDIN_FILENO);
	}
	if (i < data->nb_pipes && data->output == -1)
		dup2(data->pipe_fd[i][1], STDOUT_FILENO);
	close_pipes(data->pipe_fd, data->nb_pipes, data);
	execute_command(data->cmds[i], data->envp, token, data);
	free_pids(data);
	exit(EXIT_FAILURE);
}

void	child(t_data *data, int i, t_token *token)
{
	
	// printf("lauching child process\n");
	// print_current_token(token);
	if (data->input != -1)
	{
		dup2(data->input, STDIN_FILENO);
		close(data->input);
	}
	if (data->output != -1)
	{
		if (i == data->nb_pipes)
		{
			dup2(data->output, STDOUT_FILENO);
			close(data->output);
		}
		else
			dup2(data->pipe_fd[i][1], STDOUT_FILENO);
	}
	if (!data->nb_pipes)
	{
		execute_command(data->token->str, data->envp, token, data);
		free_pids(data);
		exit(EXIT_FAILURE);
	}
	else
		handle_pipes(data, i, token);
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

void	create_child(t_data *data, t_token *token)
{
	int	i;

	i = 0;
	// printf("nb_pipes = %d\n", data->nb_pipes);
	data->pids = safe_malloc(sizeof(pid_t) * (data->nb_pipes + 1));
	init_pids(data);
	while (i <= data->nb_pipes)
	{
		printf("create_child: i = %d, nb_pipes = %d, data->pids = %p\n", i, data->nb_pipes, (void *)data->pids);
		data->pids[i] = fork();
		if (data->pids[i] == 0)
		{
			child(data, i, token);
			exit(0);
		}
		else if (data->pids[i] < 0)
		{
			perror("fork");
			break ;
		}
		i++;
	}
	parent(data, i - 1);
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
			if (!check_and_or(current, data))
				break ;
			if (data->nb_pipes)
			{
				token_to_array(data->token, data, data->nb_pipes);
				create_child(data, current);
				free_pipes(data->pipe_fd, data->nb_pipes, data);
				return ;
			}
			create_child(data, current);
		}
		// print_current_token(current);
		current = current->next;
	}
}
