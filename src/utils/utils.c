/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 17:35:34 by tcali             #+#    #+#             */
/*   Updated: 2025/09/22 12:31:12 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_all_pipes(int **pipes, int n, t_data *data)
{
	int	i;

	i = 0;
	if (!data->nb_pipes)
		return ;
	while (i < n)
	{
		if (pipes[i][0] >= 0)
			close(pipes[i][0]);
		if (pipes[i][1] >= 0)
			close(pipes[i][1]);
		i++;
	}
}

// converts a token of type CMD to type ARG.
static void	cmd_to_arg(t_token *start, t_data *data)
{
	t_token	*current;

	current = start;
	(void)data;
	while (current
		&& (current->type == CMD || current->type == ARG))
	{
		current->type = ARG;
		if (!current->next)
			break ;
		current = current->next;
	}
}

// token->type already defined previously, double checking when all tokens have
// been parsed to see if some cmds should be defined as args.
int	check_type(t_token *token, t_data *data)
{
	t_token	*current;

	current = token;
	while (current)
	{
		if (!current->next && (current->type == AND || current->type == OR || current->type == PIPE))
		{
			print_error("syntax error near unexpected token `newline'", OFF, NULL);
			return (0);
		}
		if (current->pos == 0 && current->type != CMD)
		{
			data->last_exit = redirect_stream(token, NULL, data);
			if (data->last_exit != 1)
				return (0);
		}
		if (current->pos >= 1)
		{
			// if (current->type != CMD && current->type != ARG && current->prev->type != CMD && current->prev->type != ARG)
			// {
			// 	syntax_error(current->str);
			// 	return (0);
			// }
			while (current->join_next == true && current->next)
			{
				current = add_arg(current, data);
				// current = current->next;
				print_current_token(current);
			}
			if (!is_builtin(current->prev->str) && current->type == CMD
				&& (current->prev->type == CMD || current->prev->type == ARG))
			{
				current = add_arg(current->prev, data);
			}
			else if (is_builtin(current->prev->str) && current->type == CMD)
				cmd_to_arg(current, data);
			else if (is_redirection(current->type))
			{
				data->last_exit = redirect_stream(current, find_prev_cmd(current), data);
				if (data->last_exit != 1)
					return (0);
			}
		}
		if (!current->next)
			break ;
		current = current->next;
	}
	return (1);
}

// when && or || is present, checks if exec should be stopped.
// (return 0) if it should be stopped.
// (return 1) if it should proceed.
int	check_and_or(t_token *token, t_data *data)
{
	if (token->prev && token->prev->type == OR && data->last_exit == 0)
		return (0);
	else if (token->prev && token->prev->type == AND && data->last_exit != 0)
		return (0);
	return (1);
}

void	wait_all(t_data *data, int *last_status)
{
	int	i;
	int	status;
	int	exit_code;

	i = 0;
	exit_code = 0;
	while (i <= data->nb_pipes)
	{
		if (data->pids[i] > 0)
		{
			waitpid(data->pids[i], &status, 0);
			*last_status = status;
			if (WIFSIGNALED(status))
				exit_code = 128 + WTERMSIG(status);
			else if (i == data->nb_pipes && WIFEXITED(status))
				exit_code = WEXITSTATUS(status);
			i++;
		}
	}
	data->last_exit = exit_code;
	g_exit_status = exit_code;
}

// void	wait_all(t_data *data, int *last_status)
// {
// 	int	i;
// 	int	status;

// 	i = 0;
// 	status = 0;
// 	while (i <= data->nb_pipes)
// 	{
// 		if (data->pids[i] > 0)
// 		{
// 			waitpid(data->pids[i], &status, 0);
// 			*last_status = status;
// 			if (i == data->nb_pipes)
// 			{
// 				if (WIFEXITED(status))
// 					data->last_exit = WEXITSTATUS(status);
// 			}
// 			i++;
// 		}
// 	}
// }
