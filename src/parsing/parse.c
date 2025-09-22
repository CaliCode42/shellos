/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 19:26:29 by tcali             #+#    #+#             */
/*   Updated: 2025/09/12 16:07:19 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**fill_array(t_data *data, t_token *current, int count)
{
	int	i;

	i = 0;
	while (i < count && current)
	{
		if (current->type == CMD)
		{
			data->cmds[i] = ft_strdup(current->str);
			if (!data->cmds[i])
			{
				free_array(&data->cmds);
				perror("ft_strdup failed\n");
				exit(EXIT_FAILURE);
			}
			i++;
		}
		current = current->next;
	}
	data->cmds[i] = NULL;
	data->array_alloc = true;
	return (data->cmds);
}

// converts all the CMDs and ARGs preceding a pipe into an array**
void	token_to_array(t_token *start, t_data *data)
{
	t_token	*current;
	int		count;

	current = start;
	count = 0;
	while (current)
	{
		if (current->type == AND || current->type == OR)
			break ;
		if (current->type != PIPE)
			count++;
		current = current->next;
	}
	if (count == 0)
		return ;
	data->cmds = safe_malloc(sizeof(char *) * (count + 1));
	current = start;
	data->cmds = fill_array(data, current, count);
}

// // counting tokens, using space as a delimiter, returning nb of tokens.
// static int	count_tokens(char const *s)
// {
// 	size_t	i;
// 	int		in_word;
// 	int		word_count;

// 	i = 0;
// 	in_word = 0;
// 	word_count = 0;
// 	while (s[i])
// 	{
// 		if (!ft_isspace(s[i]) && in_word == 0)
// 		{
// 			in_word = 1;
// 			word_count++;
// 		}
// 		else if (ft_isspace(s[i]))
// 			in_word = 0;
// 		i++;
// 	}
// 	return (word_count);
// }
// 
// // ft_split on line, returning an array** of tokens;
// char	**parse_line(const char *line)
// {
// 	char		**tokens;
// 	const char	*start;
// 	int			i;
// 	int			len;

// 	tokens = safe_malloc(sizeof(char *) * (count_tokens(line) + 1));
// 	i = 0;
// 	while (*line)
// 	{
// 		while (*line && ft_isspace(*line))
// 			line++;
// 		if (!*line)
// 			break ;
// 		start = line;
// 		while (*line && !ft_isspace(*line))
// 			line++;
// 		len = line - start;
// 		tokens[i] = ft_strndup(start, len);
// 		i++;
// 	}
// 	tokens[i] = NULL;
// 	return (tokens);
// }
