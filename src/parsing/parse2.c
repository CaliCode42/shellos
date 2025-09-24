/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:21:59 by tcali             #+#    #+#             */
/*   Updated: 2025/09/25 01:13:32 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isquote(char c)
{
	if (c == '\'' || c == '"')
		return (1);
	return (0);
}

int	quote_is_closed(char *line, char quote)
{
	int		i;
	int		count_quotes;
	bool	in_other_quotes;
	bool	in_quotes;

	i = 0;
	count_quotes = 0;
	in_other_quotes = false;
	in_quotes = false;
	while (line[i])
	{
		if (ft_isquote(line[i]) && line[i] != quote && !in_quotes)
		{
			if (!in_other_quotes)
				in_other_quotes = true;
			else
				in_other_quotes = false;
		}
		else if (line[i] == quote && !in_other_quotes)
		{
			if (!in_quotes)
				in_quotes = true;
			else
				in_quotes = false;
			count_quotes++;
		}
		i++;
	}
	if (count_quotes % 2 == 0)
		return (1);
	return (0);
}

int	ft_is_sep(char c)
{
	char	*sep;

	sep = "<>&|";
	if (ft_strchr(sep, c) || c == '\0')
		return (1);
	return (0);
}

int	find_end_quote(char *line, int i, char quote)
{
	if (!quote_is_closed(line, quote))
		return (-1);
	i++;
	while (line[i] && line[i] != quote)
		i++;
	return (i + 1);
}

int	char_is_triple(char *line, char sep)
{
	int	i;

	i= 0;
	if (ft_strlen(line) < 2)
		return (0);
	while (line[i + 2])
	{
		if (line[i] == sep && line[i + 1] == sep && line[i + 2] == sep)
		{
			print_unexpected_token(sep, 2);
			return (1);
		}
		i++;
	}
	return (0);
}

int	syntax_quote(char *line, char quote)
{
	if (!quote_is_closed(line, quote))
	{
		print_unexpected_token(quote, 1);
		return (-1);
	}
	return (0);
}

int	syntax_redirect(char *line, int i, char sep)
{
	if (i == 0	&& !line[i + 1])
	{
		print_unexpected_token(sep, 1);
		return (-1);
	}
	while ((line[i] && ft_isspace(line[i])) || line[i] == sep)
		i++;
	if ((line[i] == '>' || line[i] == '<' || line[i] == '\0') && line[i] != sep)
	{
		safe_print("syntax error near unexpected token `newline'",
			false, true);
		return (-1);
	}
	if (line[i] == '|')
	{
		print_unexpected_token(sep, 1);
		return (-1);
	}
	return (0);
}

int	syntax_operator(char *line, int i)
{
	if (line[i] == '&')
	{
		if (!line[i + 1] || (line[i + 1] != '&' && line[i - 1] != '&'))
		{
			print_unexpected_token('&', 1);
			return (-1);
		}
		if (line[i + 1] == '|')
		{
			print_unexpected_token('|', 1);
			return (-1);
		}
	}
	return (0);
}

int	syntax_error(char *line, int i, char sep)
{
	if (ft_isquote(sep))
		return (syntax_quote(line, sep));
	if (char_is_triple(line, sep))
		return (-1);
	if (sep == '<' || sep == '>')
		return (syntax_redirect(line, i, sep));
	if (sep == '&' || sep == '|')
		return (syntax_operator(line, i));
	return (0);
}

int	copy_token(t_input *input, int start, int i)
{
	int	j;

	input->token = safe_malloc(sizeof(char) * (i - start) + 1);
	j = 0;
	while (input->line[start] && start < i)
	{
		input->token[j] = input->line[start];
		j++;
		start++;
	}
	input->token[j] = '\0';
	// printf("token = [%s]\n", input->token);
	return (i);
}

int parse_line(t_data *data, t_input *input)
{
	int 	i;
	int		start;
	char	*sep;
	bool	in_quote;

	i = 0;
	start = 0;
	in_quote = false;
	while (input->line[i])
	{
		while (input->line[i] && ft_isspace(input->line[i]))
			i++;
		if (!input->line[i])
			break ;
		start = i;
		if (ft_is_sep(input->line[i]))
		{
			if (syntax_error(input->line, i, input->line[i]))
			{
				data->last_exit = 2;
				return (-1);
			}
			sep = char_to_str(input->line[i++]);
			if (input->line[i] == input->line[i - 1])
			{
				ft_free((void **)&sep);
				sep = ft_strjoin_free(char_to_str(input->line[i]), char_to_str(input->line[i]));
				i += 2;
			}
			token_add_back(&data->token, new_token(sep, false));
			ft_free((void **)&sep);
		}
		else
		{
			while (input->line[i])
			{
				// printf("line[%d] = [%c]\n", i, input->line[i]);
				if (ft_isquote(input->line[i]))
				{
					if (syntax_error(input->line, i, input->line[i]))
						return (printf("line[%d] = %c\n", i, input->line[i]), -1);
					if (!in_quote)
					{
						in_quote = true;
						input->quote = input->line[i];
					}
					else
					{
						if (input->line[i] == input->quote)
						{
							in_quote = false;
							i++;
							break ;
						}
					}
				}
				if (!in_quote && (ft_isspace(input->line[i])
						|| ft_is_sep(input->line[i])))
					break ;
				i++;
			}
			copy_token(input, start, i);
			if (ft_isspace(input->line[i]) || ft_is_sep(input->line[i]) || input->line[i] == '\0')
				token_add_back(&data->token, new_token(input->token, false));
			else
				token_add_back(&data->token, new_token(input->token, true));
			ft_free((void **)&input->token);
		}
	}
	return (1);
}

char	*remove_quotes(char *token, char quote)
{
	int		i;
	int		j;
	int		count_quote;
	char	*tmp;

	i = 0;
	count_quote = 0;
	while (token[i])
	{
		if (token[i] == quote)
			count_quote++;
		i++;
	}
	// printf("token = [%s]\n", token);
	// printf("i = %d\n", i);
	// printf("count_quote = %d\n", count_quote);
	tmp = safe_malloc(sizeof(char) * (i - count_quote + 1));
	i = 0;
	j = 0;
	while (token[i])
	{
		if (token[i] != quote)
		{
			tmp[j] = token[i];
			j++;
		}
		i++;
	}
	tmp[j] = '\0';
	ft_free((void **)&token);
	return (tmp);
}

char	*expand_value(char *str, int start, int i)
{
	char	*tmp;
	int		j;

	j = 0;
	tmp = safe_malloc(sizeof(char) * (i - start) + 1);
	while (str[start] && start < i)
	{
		tmp[j] = str[start];
		j++;
		start++;
	}
	tmp[j] = '\0';
	return (tmp);
}

char	*copy_remaining(char *str, int i)
{
	char	*remaining;
	int		start;
	int		j;

	remaining = NULL;
	start = i;
	while (str[i])
		i++;
	remaining = safe_malloc(sizeof(char) * (i - start) + 1);
	j = 0;
	while (start < i)
	{
		remaining[j] = str[start];
		j++;
		start++;
	}
	remaining[j] = '\0';
	return (remaining);
}

static int	var_len(char *str)
{
	int	i;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}

char	*var_to_value(char *var, t_env *env)
{
	t_env	*current;
	int		len_var;

	len_var = ft_strlen(var);
	current = env;
	while (current)
	{
		if (!ft_strncmp(current->variable, var, len_var) && current->variable[len_var] == '\0')
			return (ft_strdup(current->value));
		current = current->next;
	}
	if (!ft_strncmp(var, "~", 2))
		return (ft_strdup(var));
	return (NULL);
}

static char	*expand_var(char *str, int *i, t_data *data)
{
	char	*name;
	char	*value;
	int		len;

	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(data->last_exit));
	}
	len = var_len(&str[*i]);
	name = ft_substr(str, *i, len);
	*i += len;
	// value = ft_strdup(name);
	value = var_to_value(name, data->exported_env);
	if (!value)
		value = ft_strdup("");
	free(name);
	return (value);
}

static char	*append_char(char *res, char c)
{
	char	buf[2];
	char	*tmp;

	buf[0] = c;
	buf[1] = '\0';
	tmp = ft_strdup(buf);
	res = ft_strjoin_free(res, tmp);
	return (res);
}

int	in_single_quotes(char *str, int i)
{
	int		j;
	bool	in_double_quotes;

	j = 0;
	in_double_quotes = false;
	while (j < i)
	{
		if (str[j] == '"')
		{
			if (!in_double_quotes)
				in_double_quotes = true;
			else
				in_double_quotes = false;
		}
		else if (str[j] == '\'' && !in_double_quotes)
			return (1);
		j++;
	}
	return (0);
}

int	to_expand(char *str, int i)
{
	if (!ft_isalnum(str[i + 1]) && str[i + 1] != '_' && str[i + 1] != '?')
		return (0);
	if (in_single_quotes(str, i))
		return (0);
	return (1);
}

char	*expand_dollar(char *str, t_data *data)
{
	int		i;
	char	*res;
	char	*tmp;

	i = 0;
	if (!ft_strncmp(str, "$", 2) || !ft_strncmp(str, "\'$\'", 4) || !ft_strncmp(str, "\"$\"", 4))
		return (str);
	// if (only_dollar(str))
	// 	return (str);
	res = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '$' && to_expand(str, i))
		{
			i++;
			tmp = expand_var(str, &i, data);
			res = ft_strjoin_free(res, tmp);
		}
		else
		{
			res = append_char(res, str[i]);
			i++;
		}
	}
	return (ft_free((void **)&str), res);
}

// char	*expand_dollar(char *str)
// {
// 	int 	i;
// 	int		start;
// 	char	*expanded;
// 	char	*tmp;

// 	i = 0;
// 	start = 0;
// 	expanded = NULL;
// 	tmp = ft_strdup("");
// 	while (str[i])
// 	{
// 		if (str[i] == '$')
// 		{
// 			start = i;
// 			while (ft_isalnum(str[i]) || str[i] == '_')
// 				i++;
// 			if (start > 0)
// 				tmp = ft_strndup(str, start);
// 			expanded = expand_value(str, start, i);
// 			if (tmp)
// 				expanded = ft_strjoin_free(tmp, expanded);
// 			if (str[i] != '\0')
// 			{
// 				tmp = copy_remaining(str, i);
// 				i += (start - ft_strlen(expanded));
// 				ft_free((void **)&str);
// 				str = ft_strjoin_free(expanded, tmp);
// 			}
// 			else
// 			{
// 				ft_free((void **)&str);
// 				str = ft_strdup(expanded);
// 				ft_free((void **)&expanded);
// 			}
// 		}
// 		else
// 			i++;
// 	}
// 	return (str);
// }

int	parse_token(t_token *current, t_data *data)
{
	int	i;

	i = 0;
	// printf("[%s]\n", current->str);
	if (ft_strchr(current->str, '$') && ft_strncmp(current->str, "$", 2))
		current->to_expand = true;
	if (current->to_expand)
	{
		current->str = expand_dollar(current->str, data);
		current->to_expand = false;
	}
	// printf("\t[%s]\n", current->str);
	while (current->str[i])
	{
		if (ft_isquote(current->str[i]))
		{
			if (current->str[i] == '\'')
				current->to_expand = false;
			current->str = remove_quotes(current->str, current->str[i]);
			break ;
		}
		i++;
	}
	return (0);
}

int	parse_all_tokens(t_data *data)
{
	t_token	*current;

	current = data->token;
	while (current)
	{
		if (parse_token(current, data) == -1)
			return (-1);
		// print_current_token(current);
		if (current->prev && current->prev->join_next)
			current = add_arg(current->prev, data);
		current = current->next;
	}
	return (1);
}

// int	find_end_sep(char *line, int i, char sep)
// {
// 	if (syntax_error(line, i, sep))
// 		return (-1);
// 	if (line[i + 1] != sep)
// 		return (i + 1);
// 	return (i + 2);
// }

// int	find_end_dollar(char *line, int i)
// {
// 	if (ft_isspace(line[i + 1]))
// 		return (i + 1);
// 	if (line[i + 1] == '?')
// 		return (i + 2);
// 	while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
// 		i++;
// 	return (i);
// }

// int	find_end_of_token(char *line, int i)
// {
// 	if (line[i] == '\'' || line[i] == '\"')
// 		i = find_end_quote(line, i, line[i]);
// 	else if (ft_strchr("<>|&", line[i]))
// 		i = find_end_sep(line, i, line[i]);
// 	else if (line[i] == '$')
// 		i = find_end_dollar(line, i);
// 	return (i);
// }

// int	parse_line(t_data *data, t_input *input)
// {
// 	t_token	*new;
// 	int		i;
// 	int		start;

// 	new = NULL;
// 	i = 0;
// 	start = 0;
// 	while (input->line[i])
// 	{
// 		while (input->line[i] && ft_isspace(input->line[i]))
// 		{
// 			printf("\tline[%d] = [%c]\n", i, input->line[i]);
// 			i++;
// 		}
// 		if (!input->line[i])
// 			break ;
// 		start = i;
// 		while (input->line[i] && !ft_isspace(input->line[i]))
// 		{
// 			printf("line[%d] = [%c]\n", i, input->line[i]);
// 			if (input->in_token)
// 				printf("in_token = true\n");
// 			else
// 				printf("in_token = false\n");
// 			if (!ft_is_sep(input->line[i]))
// 				input->in_token = true;
// 			else
// 			{
// 				// printf("start = %d\ti = %d\n", start, i);
// 				i = copy_token(input, start, i);
// 				if (i == -1)
// 					return (0);
// 				new = new_token(input->token);
// 				token_add_back(&data->token, new);
// 				ft_free((void **)&input->token);
// 				input->in_token = false;
// 				start = i;
// 			}
// 			i++;
// 		}
// 		printf("start = %d\ti = %d\n", start, i);
// 		copy_token(input, start, i);
// 	}
// 	return (1);
// }
