/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 19:26:29 by tcali             #+#    #+#             */
/*   Updated: 2025/06/19 12:01:52 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_tokens(char const *s)
{
	size_t	i;
	int		in_word;
	int		word_count;

	i = 0;
	in_word = 0;
	word_count = 0;
	while (s[i])
	{
		if (!ft_isspace(s[i]) && in_word == 0)
		{
			in_word = 1;
			word_count++;
		}
		else if (ft_isspace(s[i]))
			in_word = 0;
		i++;
	}
	return (word_count);
}

char	**parse_line(const char *line)
{
	char		**tokens;
	const char	*start;
	int			i;
	int			len;

	tokens = safe_malloc(sizeof(char *) * (count_tokens(line) + 1));
	i = 0;
	while (*line)
	{
		while (*line && ft_isspace(*line))
			line++;
		if (!*line)
			break ;
		start = line;
		while (*line && !ft_isspace(*line))
			line++;
		len = line - start;
		tokens[i] = ft_strndup(start, len);
		i++;
	}
	tokens[i] = NULL;
	return (tokens);
}
