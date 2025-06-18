/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 17:35:34 by tcali             #+#    #+#             */
/*   Updated: 2025/06/18 16:22:22 by tcali            ###   ########.fr       */
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
		}
		current = current->next;
	}
}

void	add_arg(t_token *current)
{
	t_token	*new_next;

	new_next = NULL;
	if (!current->next || !current->next->str)
		return ;
	if (current->next->next)
		new_next = current->next->next;
	current->str = ft_str_threejoin(current->str, " ", current->next->str);
	free(current->next->str);
	free(current->next);
	current->next = new_next;
}
