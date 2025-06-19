/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 11:35:11 by tcali             #+#    #+#             */
/*   Updated: 2025/06/19 15:09:03 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child(t_data *data, int i)
{
	if (!data->nb_pipes)
	{
		printf("command = %s\n", data->token->str);
		execute_command(data->token->str, data->envp);
		exit(EXIT_FAILURE);
	}
	else
	{
		if (i > 0)
			dup2(data->pipe_fd[i - 1][0], STDIN_FILENO);
		if (i < data->nb_pipes)
			dup2(data->pipe_fd[i][1], STDOUT_FILENO);
		close_pipes(data->pipe_fd, data->nb_pipes - 1, data);
		printf("command = %s\n", data->tokens[i]);
		execute_command(data->tokens[i], data->envp);
		exit(EXIT_FAILURE);
	}
}

void	parent(t_data *data, int i)
{
	int	status;
	int	sig;

	(void)i;
	if (data->nb_pipes && i == data->nb_pipes)
	{
		close_pipes(data->pipe_fd, data->nb_pipes, data);
		free_pipes(data->pipe_fd, data->nb_pipes, data);
		//data->nb_pipes = 0;
		if (data->array_alloc == true)
		{
			free_array(data->tokens);
			data->array_alloc = false;
		}
	}
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

void	token_to_array(t_token *token, t_data *data, int n)
{
	int		i;
	t_token	*current;

	i = 0;
	current = token;
	data->tokens = safe_malloc(sizeof(char *) * (n + 1));
	while (i < n + 1 && current)
	{
		if (!ft_strncmp(current->str, "|", 2))
		{
			if (!current->next)
			{
				//need to print new prompt waiting for pipe instruction
				//(pipe>)
				break ;
			}
			current = current->next;
		}
		data->tokens[i] = ft_strdup(current->str);
		i++;
		current = current->next;
	}
	i = 0;
	while (data->tokens[i])
	{
		printf("tokens[%d] = %s\n", i, data->tokens[i]);
		i++;
	}
	data->array_alloc = true;
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
				printf("cmd = %s\n", current->str);
				if (current->next && current->next->str)
					printf("next cmd = %s\n", current->next->str);
				while (current->next && current->next->type == ARG)
					add_arg(current);
				if (data->nb_pipes)
				{
					token_to_array(data->token, data, data->nb_pipes);
				}
				create_child(data);
			}
		}
		current = current->next;
	}
}
