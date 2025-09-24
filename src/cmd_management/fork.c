/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 11:35:11 by tcali             #+#    #+#             */
/*   Updated: 2025/09/25 01:09:51 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	handle_pipes(t_data *data, int i, t_token *token)
// {
// 	if (i > 0 && i < data->nb_cmds)
// 	{
// 		if (current->input == -1)
// 			dup2(data->pipe_fd[i - 1][0], STDIN_FILENO);
// 	}
// 	else if (i < data->nb_cmds && data->output == -1)
// 		dup2(data->pipe_fd[i][1], STDOUT_FILENO);
// 	close_pipes(data->pipe_fd, data->nb_pipes, data);
// 	if (token->type != CMD)
// 		return ;
// 	execute_command(data->cmds[i], data->envp, token, data);
// 	free_pids(data);
// 	exit(EXIT_FAILURE);
// }

void	child(t_data *data, int i, t_token *token)
{
	(void)i;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (token->input != -1)
	{
		dup2(token->input, STDIN_FILENO);
		close(token->input);
	}
	if (token->output != -1)
	{
		dup2(token->output, STDOUT_FILENO);
		close(token->output);
	}
	execute_command(token->str, data->envp, token, data);
	// free_minishell(data, true);
	// free_pids(data);
	exit(EXIT_FAILURE);
}

void	parent(t_data *data)
{
	int	status;

	status = 0;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (data->nb_pipes)
		wait_all(data, &status);
	else
	{
		waitpid(data->pid, &status, 0);
		if (WIFEXITED(status))
			data->last_exit = WEXITSTATUS(status);
	}
	free_pipes(data->pipe_fd, data->nb_pipes, data);
	// free_pids(data);
	// ft_free((void **)&data->pid);
	// free_minishell(data, false);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	check_sigint_sigquit(status, data);
}

void	create_child(t_data *data, t_token *token)
{
	data->pid = fork();
	if (data->pid == 0)
	{
		child(data, 0, token);
		exit(0);
	}
	else if (data->pid < 0)
	{
		perror("fork");
		return ;
	}
	parent(data);
	if (data->array_alloc == true)
	{
		free_array(&data->cmds);
		data->array_alloc = false;
	}
	// ft_free((void **)&data->pid);
}

// if nothing should be executed (return -1)
// creating a pipeline if data->nb_pipes > 0. (return 1)
// exec_builtin directly if no_fork_needed. (return 0)
// else doing a simple fork. (return 0)
static t_token	*check_process_needed(t_data *data, t_token *current)
{
	data->nb_pipes = count_pipes(data, current, false);
	// printf("nb of pipes =  %d\n", data->nb_pipes);
	if (!check_and_or(current, data))
		return (NULL);
	if (data->nb_pipes > 0)
	{
		current = create_pipeline(data, current);
		if (!current)
			return (free_minishell(data, false), NULL);
		return (current);
	}
	else
	{
		if (is_builtin(current->str) && no_fork_needed(current->str))
		{
			if (exec_builtin(current, data) == 0)
				data->last_exit = 0;
		}
		else
			create_child(data, current);
	}
	if (!current)
		return (free_minishell(data, false), NULL);
	return (current->next);
}

// checks which kind of process is needed to execute the cmd line.
void	fork_process(t_data *data)
{
	t_token	*current;
	// char	*tmp;

	current = data->token;
	while (current)
	{
		// if (current->to_expand == true)
		// {	
		// 	tmp = expand_quotes(current->str, data, current);
		// 	ft_free((void **)&current->str);
		// 	current->str = tmp;
		// }
		// print_current_token(current);
		if (current->pos == 0 && current->type == HEREDOC)
			check_heredoc(current, NULL, data);
		if (current->type == CMD)
		{
			current = check_process_needed(data, current);
			if (current == NULL)
				break ;
		}
		else
			current = current->next;
		if (data->total_pipes && data->nb_pipes < data->total_pipes)
		{
			data->nb_pipes = count_pipes(data, current, false);
			data->nb_cmds = count_cmds(current);
		}
	}
}
