/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 17:35:34 by tcali             #+#    #+#             */
/*   Updated: 2025/06/19 15:08:14 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_pipes(int **pipes, int n, t_data *data)
{
	int	i;

	i = 0;
	if (!data->nb_pipes)
		return ;
	while (i < n)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

void	check_type(t_token *token)
{
	t_token	*current;

	current = token;
	while (current)
	{
		if (current->pos >= 1)
		{
			//printf("%d", current->prev->type);
			if (current->type == CMD
				&& (current->prev->type == CMD || current->prev->type == ARG))
			{
				current->type = ARG;
			}
			while (current->next && current->next->type == ARG)
				add_arg(current);
		}
		current = current->next;
	}
}
