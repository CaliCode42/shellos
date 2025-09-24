/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_errors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:58:51 by chdoe             #+#    #+#             */
/*   Updated: 2025/09/16 13:14:09 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parsing.h"

int	is_bad_redirect(char c)
{
	if (c == 0 || c == '<' || c == '>' || c == '|')
		return (1);
	return (0);
}

int	syntax_error(t_token *current)
{
	if (!current->str)
		return (1);
	if (redirect_inout(current->str) == -1)
	{
		return (2);
	}
	if (append_inout(current->str) == -1)
	{
		return (2);
	}
	return (0);
}

//	perror a utiliser seulement si besoin d'une info liee a errno