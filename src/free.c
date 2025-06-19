/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 11:31:56 by tcali             #+#    #+#             */
/*   Updated: 2025/06/19 12:10:03 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	free_minishell(t_data *data)
{
	if (!data)
		return ;
	free_list(&data->token);
	rl_clear_history();
}

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

void	free_list(t_token **lst)
{
	t_token	*tmp;

	while (*lst)
	{
		tmp = (*lst)->next;
		free((*lst)->str);
		free(*lst);
		*lst = tmp;
	}
}

void	free_pipes(int **pipes, int n, t_data *data)
{
	int	i;

	i = 0;
	if (!data->nb_pipes)
		return ;
	while (i < n)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
}
