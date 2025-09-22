/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:45:29 by tcali             #+#    #+#             */
/*   Updated: 2025/09/02 18:27:20 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_exported(t_env *exported)
{
	t_env	*tmp;

	while (exported)
	{
		tmp = exported->next;
		ft_free((void **)&exported->variable);
		ft_free((void **)&exported->value);
		ft_free((void **)&exported);
		exported = tmp;
	}
}
