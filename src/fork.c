/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 11:35:11 by tcali             #+#    #+#             */
/*   Updated: 2025/06/23 13:56:34 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child(t_data *data, int i)
{
	if (!data->nb_pipes)
	{
		printf("!nb_pipes\ncommand = %s\n", data->token->str);
		execute_command(data->token->str, data->envp);
		free_pids(data);
		exit(EXIT_FAILURE);
	}
	else
	{
		if (i > 0)
			dup2(data->pipe_fd[i - 1][0], STDIN_FILENO);
		if (i < data->nb_pipes)
			dup2(data->pipe_fd[i][1], STDOUT_FILENO);
		close_pipes(data->pipe_fd, data->nb_pipes, data);
		ft_printf_fd(1, "cmds[%d] = %s\n", i, data->cmds[i]);
		execute_command(data->cmds[i], data->envp);
		free_pids(data);
		exit(EXIT_FAILURE);
	}
}

void	wait_all(t_data *data, int *last_status)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i <= data->nb_pipes)
	{
		if (data->pids[i] > 0)
		{
			waitpid(data->pids[i], &status, 0);
			*last_status = status;
		}
		i++;
	}
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
				//printf("cmd = %s\n", current->str);
				//if (current->next && current->next->str)
				//	printf("next cmd = %s\n", current->next->str);
				//while (current->next && current->next->type == ARG)
				//	add_arg(current);
				if (data->nb_pipes)
				{
					printf("calling tkn to arr\n");
					token_to_array(data->token, data, data->nb_pipes);
					create_child(data);
					free_pipes(data->pipe_fd, data->nb_pipes, data);
					return ;
				}
				create_child(data);
				//free(data->pids);
			}
		}
		current = current->next;
	}
}
