/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 15:14:40 by tcali             #+#    #+#             */
/*   Updated: 2025/07/18 15:14:40 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// child process which redirects input and output where needed,
// then closes all pipes and executing the cmd
void	pipeline_childs(t_token *current, t_data *data, int i)
{
	printf("pipe_child\n");
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (current->input != -1)
	{
		dup2(current->input, STDIN_FILENO);
		close(current->input);
	}
	else if (i > 0 && i <= data->nb_pipes)
	{
		dup2(data->pipe_fd[i - 1][0], STDIN_FILENO);
		close(data->pipe_fd[i - 1][0]);
	}
	if (current->output != -1)
	{
		dup2(current->output, STDOUT_FILENO);
		close(current->output);
	}
	else if (i < (data->nb_pipes))
	{
		dup2(data->pipe_fd[i][1], STDOUT_FILENO);
		close(data->pipe_fd[i][1]);
	}
	close_all_pipes(data->pipe_fd, (data->nb_pipes), data);
	if (current->type == CMD)
		execute_command(data->cmds[0], data->envp, current, data);
	exit(EXIT_FAILURE);
}

void	close_pipe(int i, t_data *data)
{
	if (i > 0 && i <= data->nb_pipes)
		close(data->pipe_fd[i - 1][0]);
	if (i < data->nb_pipes)
		close(data->pipe_fd[i][1]);
}

// returns the next token to be executed after '|', '&&' or '||'.
t_token	*next_to_execute(t_token *current)
{
	while (current && (current->type != PIPE && current->type != AND
			&& current->type != OR))
		current = current->next;
	if (current && (current->type == PIPE || current->type == AND
			|| current->type == OR))
		current = current->next;
	return (current);
}

t_token	*launch_pipes_process(t_data *data, t_token *current)
{
	pid_t	pid;
	int		i;

	i = 0;
	while (i < data->nb_cmds && current)
	{
		if (i > data->nb_pipes)
			return (current);
		token_to_array(current, data);
		// print_array(data->cmds, true);
		pid = fork();
		if (pid == 0)
			pipeline_childs(current, data, i);
		else if (pid < 0)
		{
			perror("fork failed");
			break ;
		}
		data->pids[i] = pid;
		close_pipe(i, data);
		current = next_to_execute(current);
		free_array(&data->cmds);
		data->array_alloc = false;
		i++;
	}
	close_all_pipes(data->pipe_fd, (data->nb_pipes), data);
	parent(data);
	return (current);
}

// creates a pipeline which will execute the cmds and
// write the output on the next pipe's input.
t_token	*create_pipeline(t_data *data, t_token *start)
{
	t_token	*current;

	current = start;
	data->pids = safe_malloc(sizeof(pid_t) * (data->nb_pipes + 1));
	init_pids(data);
	init_pipes(data, data->nb_pipes);
	current = launch_pipes_process(data, current);
	return (current);
}
