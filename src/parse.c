/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 19:26:29 by tcali             #+#    #+#             */
/*   Updated: 2025/06/23 13:44:23 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	token_to_array(t_token *token, t_data *data, int n)
{
	int		i;
	t_token	*current;

	i = 0;
	current = token;
	data->cmds = safe_malloc(sizeof(char *) * (n + 2));
	while (i < n + 1 && current)
	{
		if (!ft_strncmp(current->str, "|", 2))
		{
			if (!current->next)
			{
				//need to print new prompt waiting for pipe instruction
				//(pipe>)
				break ;
			}
			current = current->next;
		}
		printf("(%d)current->str = %s\n", current->pos, current->str);
		data->cmds[i] = ft_strdup(current->str);
		if (!data->cmds[i])
		{
			free_array(data->tokens, 0);
			perror("ft_strdup failed\n");
		}
		i++;
		current = current->next;
	}
	data->cmds[i] = NULL;
	i = 0;
	while (data->cmds[i])
	{
		printf("cmds[%d] = %s\n", i, data->cmds[i]);
		i++;
	}
	data->array_alloc = true;
}

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
