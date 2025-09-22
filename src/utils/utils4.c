/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 12:12:31 by tcali             #+#    #+#             */
/*   Updated: 2025/09/12 17:09:58 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_tokens(t_token *token)
{
	int		count;
	t_token	*current;

	count = 0;
	current = token;
	while (current)
	{
		current = current->next;
		count++;
	}
	// printf("nb_tokens = %d\n", count);
	return (count);
}
