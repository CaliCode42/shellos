/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 11:35:11 by tcali             #+#    #+#             */
/*   Updated: 2025/06/19 12:15:26 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child(t_data *data, int i)
{
	if (i > 0)
		dup2(data->pipe_fd[i - 1][0], STDIN_FILENO);
	if (i < data->nb_pipes - 1)
		dup2(data->pipe_fd[i][1], STDOUT_FILENO);
	close_pipes(data->pipe_fd, data->nb_pipes - 1, data);
	execute_command(data->token->str, data->envp);
	exit(EXIT_FAILURE);
}

void	parent(t_data *data, int i)
{
	int	status;
	int	sig;

	(void)i;
	close_pipes(data->pipe_fd, data->nb_pipes, data);
	free_pipes(data->pipe_fd, data->nb_pipes, data);
	waitpid(data->pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
			printf("Quit (Core dumped)\n");
		else if (sig == SIGINT)
			printf("\n");
	}
}

void	create_child(t_data *data)
{
	int	i;

	i = 0;
	init_pipes(data->pipe_fd, data->nb_pipes);
	while (i <= data->nb_pipes)
	{
		printf("nb_pipes = %d\n", data->nb_pipes);
		data->pid = fork();
		if (data->pid == 0)
			child(data, i);
		else if (data->pid > 0)
			parent(data, i);
		else
		{
			perror("fork");
			break ;
		}
		i++;
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
				while (current->next && current->next->type == ARG)
					add_arg(current);
				create_child(data);
			}
		}
		current = current->next;
	}
}
