/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_errors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:58:51 by chdoe             #+#    #+#             */
/*   Updated: 2025/09/17 13:48:42 by tcali            ###   ########.fr       */
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

int	syntax_pipe(char *str)
{
	if (str[0] != '|')
		return (1);
	else if (!ft_strncmp(str, "||", 3) || !ft_strncmp(str, "|", 2))
		return (1);
	else
		return (0);
}

int	syntax_and(char *str)
{
	if (str[0] != '&')
		return (1);
	else if (!ft_strncmp(str, "&&", 3))
		return (1);
	else
		return (0);
}

// int	syntax_and(char *str, t_token *current)
// {
// 	if (ft_strchr(str, '&') && str[0] != '&')
// 	{
// 		if (!ft_strncmp(str, "&&", 3))
// 		{
// 			if (!current->next || !current->next->str)
// 			{
// 				print_error("syntax error near unexpected token `newline'", OFF, NULL);
// 				return (2);
// 			}
// 			return (0);
// 		}
// 		if (!triple_check(str, '&'))
// 			print_error("syntax error near unexpected token `&&'", OFF, NULL);
// 		else
// 			print_error("syntax error near unexpected token `&'", OFF, NULL);
// 		return (2);
// 	}
// 	return (0);
// }

int	syntax_error(char *str)
{
	if (!str)
		return (1);
	if (ft_strchr(str, '|') && !syntax_pipe(str))
	{
		if (!triple_check(str, '|'))
			print_error("syntax error near unexpected token `||'", OFF, NULL);
		else
			print_error("syntax error near unexpected token `|'", OFF, NULL);
		return (2);
	}
	if (ft_strchr(str, '&') && !syntax_and(str))
	{
		if (!triple_check(str, '&'))
			print_error("syntax error near unexpected token `&&'", OFF, NULL);
		else
			print_error("syntax error near unexpected token `&'", OFF, NULL);
		return (2);
	}
	if (redirect_inout(str) == -1)
	{
		print_error("syntax error near unexpected token `newline'", OFF, NULL);
		return (2);
	}
	if (append_inout(str) == -1)
	{
		print_error("syntax error near unexpected token `newline'", OFF, NULL);
		return (2);
	}
	return (0);
}

//	perror a utiliser seulement si besoin d'une info liee a errno