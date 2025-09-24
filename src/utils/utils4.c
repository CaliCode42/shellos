/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 12:12:31 by tcali             #+#    #+#             */
/*   Updated: 2025/09/24 16:43:48 by tcali            ###   ########.fr       */
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

char	*char_to_str(char c)
{
	char	*str;

	str = safe_malloc(sizeof(char *) * 2);
	str[0] = c;
	str[1] = '\0';
	return (str);
}

void	print_unexpected_token(char sep, int nb)
{
	char	*str_sep;
	char	*char_sep;

	str_sep = NULL;
	if (sep == '\0')
		safe_print("syntax error near unexpected token `newline'",
			false, false);
	char_sep = char_to_str(sep);
	if (nb == 2)
		str_sep = ft_strjoin(char_sep, char_sep);
	else
		str_sep = ft_strdup(char_sep);
	ft_free((void **)&char_sep);
	safe_print("syntax error near unexpected token `", false, false);
	safe_print(ft_strjoin(str_sep, "\'"), true, true);
	ft_free((void **)&str_sep);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	ft_free((void **)&s1);
	ft_free((void **)&s2);
	return (joined);
}
