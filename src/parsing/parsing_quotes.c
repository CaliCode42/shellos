/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_quotes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 11:11:39 by tcali             #+#    #+#             */
/*   Updated: 2025/09/24 20:55:22 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*copy_token(t_input *input, int start, int i, t_data *data)
{
	char	*token;
	char	quote;
	int		j;

	token = safe_malloc(sizeof(char) * (i - start + 2));
	j = 0;
	quote = '\0';
	if (start == i)
		i++;
	while (start < i)
	{
		token[j] = input->line[start];
		j++;
		start++;
	}
	token[j] = '\0';
	if (input->line[start] && (input->line[start] == '"' || input->line[start] == '\''))
	{
		quote = input->line[start];
		if (quote == '\'')
			data->expand = false;
		token = remove_quotes(token, quote);
	}
	return (token);
}

// 	if (token[0] == '$' && data->expand == true)
// {
// 	tmp = expand_quotes(token, data);
// 	if (!tmp)
// 		return (ft_free((void **)&token), NULL);
// 	ft_free((void **)&token);
// 	token = tmp;
// }

int	quote_is_closed(char *line, char quote)
{
	int	i;
	int	count_quotes;

	i = 0;
	count_quotes = 0;
	while (line[i])
	{
		if (line[i] == quote)
			count_quotes++;
		i++;
	}
	if (count_quotes % 2 == 0)
		return (1);
	return (0);
}

// void	read_until_closed(char quote, t_input *input)
// {
// 	char	*tmp;

// 	tmp = ft_strdup(input->line);
// 	while (1)
// 	{
// 		input->new_line = readline("> ");
// 		tmp = join_free(tmp, input->new_line);
// 		free(input->new_line);
// 		if (quote_is_closed(tmp, quote))
// 			break ;
// 	}
// 	free(input->line);
// 	input->line = ft_strdup(tmp);
// 	// input->remaining = ft_strdup(ft_strchr(tmp, quote));
// 	free(tmp);
// }

// void	parse_remaining(t_data *data, t_input *input)
// {
// 	if (!input->remaining)
// 		return ;
	
// }

// demarre sur le caractere qui suit la quote.
// avance jusau'au prochain quote :
// 1) si trouve ----> copie dans un nouveau token et l'ajoute a la liste
// 2) si pas trouve -----> readline jusqu'a le trouver (ou fin de prgm)
// dans le cas 2 il y a 2 options : 
// 1) rien apres
// 2) qqch apres
// si qqch apres il faut retourner ce qu'il reste a lire.
int	check_quotes(t_data *data, t_input *input, int start, char quote)
{
	int		i;
	char	*token;
	t_token	*new;

	i = start + 1;
	if (quote == '\'')
		data->expand = false;
	while (input->line[i] && input->line[i] != quote)
	{
		// printf("line[%d] = %c\n", i, input->line[i]);
		i++;
	}
	// printf("line[%d] = %c\n", i, input->line[i]);
	// printf("quote = %c\n", quote);
	if (input->line[i] == quote)
	{
		token = copy_token(input, (start + 1), (i), data);
		new = new_token(token);
		if (is_redirection(new->type))
			new->type = CMD;
		if ((token[0] == '$' && data->expand == true))
			new->to_expand = true;
		token_add_back(&data->token, new);
		ft_free((void **)&token);
		return (i + 1);
	}
	if (input->line[i] == '\0')
	{
		print_error("syntax error near unexpected token `newline'", OFF, NULL);
		return (free_minishell(data, false), -1);
		// safe_print("syntax error near unexpected token `newline'", false, true);
		// read_until_closed(quote, input);
		// parse_remaining(data, input);
	}
	return (i);
}

int	ft_is_sep(char c)
{
	char	*sep;

	sep = "&|$\'\"";
	if (ft_isspace(c) || ft_strchr(sep, c))
		return (1);
	return (0);
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

char	*sep_to_token(char *line, int i, t_data *data)
{
	char	sep;
	int		start;

	start = i;
	if (ft_strchr("&|", line[i]))
	{
		if (i == 0)
		{
			print_error("syntax error near unexpected token", OFF, NULL);
			data->last_exit = 2;
			free_minishell(data, false);
			return (NULL);
		}
		sep = line[start];
		if (line[i + 1] != sep && line[i - 1] != sep)
			return (char_to_str(sep));
		else if (line[i + 1] == sep && line[i + 2] != sep)
			return (ft_strjoin(char_to_str(sep), char_to_str(sep)));
		else
		{
			ft_putstr_fd("syntax error near unexpected token", 2);
			data->last_exit = 2;
			return (NULL);
		}
	}
	return (NULL);
}

int	check_separators(t_input *input, int start, int i, t_data *data)
{
	t_token	*new;

	new = NULL;
	if (input->line[i] == '$')
	{
		start = i;
		i++;
		if (input->line[i] == '?')
		{
			input->token = ft_strdup("$?");
			return (i++);
		}	
		else
		{
			while (input->line[i] && (ft_isalnum(input->line[i]) || input->line[i] == '_'))
			{
				// printf("\tline[%d] = [%c]\n", i, input->line[i]);
				i++;
			}
			input->token = copy_token(input, start, i, data);
		}
	}
	else if (ft_is_sep(input->line[i]) && !ft_isspace(input->line[i]) && input->line[i] != '\0')
	{
		input->token = sep_to_token(input->line, i, data);
		if (!input->token)
			return (-1);
		i += ft_strlen(input->token);
	}
	// else
	// 	input->token = copy_token(input, start, i, data);
	// printf("start = %d\ti = %d\n", start, i);
	if (input->token)
	{
		printf("token : [%s]\n", input->token);
		new = new_token(input->token);
		if (input->token[0] == '$' && data->expand == true)
			new->to_expand = true;
		if (!ft_isspace(input->line[i]) && input->line[i] != '\0')
			new->join_next = true;
		token_add_back(&data->token, new);
		ft_free((void **)&input->token);
	}
	return (i);
}

// parcourir la ligne. 
// si on trouve un quote on avance jusqu'au prochain. S'il n'est pas ferme on fait readline jusqu'a fermeture ou sigquit
// si il est ferme on cree un nv token dont la str contient ce qui est entre quote.
// 
// tant qu'on n'est pas entre des quotes on cree un nv token a chaque espace.
// int	parse_line(t_data *data, t_input *input)
// {
// 	t_token		*new;
// 	int			i;
// 	int			start;

// 	i = 0;
// 		if (!quote_is_closed(input->line, '"'))
// 	{
// 		data->last_exit = 2;
// 		print_error("syntax error near unexpected token `\"'", OFF, NULL);
// 		return (-1);
// 	}
// 	if (!quote_is_closed(input->line, '\''))
// 	{
// 		data->last_exit = 2;
// 		print_error("syntax error near unexpected token `\'\"", OFF, NULL);
// 		return (-1);
// 	}
// 	if (!ft_strncmp(input->line, "", 1))
// 		return (-1);
// 	if (!ft_strncmp(input->line, "\'\'", 3) || !ft_strncmp(input->line, "\"\"", 3))
// 	{
// 		print_error("command not found", OFF, NULL);
// 		data->last_exit = 127;
// 		return (-1);
// 	}
// 	if (!ft_strncmp(input->line, ":", 1))
// 	{
// 		data->last_exit = 0;
// 		return (-1);
// 	}
// 	if (check_empty_pipes(input->line) != 1)
// 	{
// 		data->last_exit = 2;
// 		// printf("\tdata->last_exit = %d\n", data->last_exit);
// 		// free_minishell(data, false);
// 		return (-1);
// 	}
// 	while (input->line && input->line[i])
// 	{
// 		data->expand = true;
// 		while (input->line[i] && ft_isspace(input->line[i]))
// 			i++;
// 		if (!input->line[i])
// 			break ;
// 		if (input->line[i] == '\'' || input->line[i] == '"')
// 		{
// 			i = check_quotes(data, input, i, input->line[i]);
// 			if (i == -1)
// 				return (-1);
// 		}
// 		else
// 		{
// 			start = i;
// 			while (input->line[i] && !ft_is_sep(input->line[i]))
// 			{
// 				// printf("line[%d] = [%c]\n", i, input->line[i]);
// 				i++;
// 			}
// 			input->token = copy_token(input, start, i, data);
// 			if (input->token)
// 			{
// 				printf("token : [%s]\n", input->token);
// 				new = new_token(input->token);
// 				if (input->token[0] == '$' && data->expand == true)
// 					new->to_expand = true;
// 				if (!ft_isspace(input->line[i]) && input->line[i] != '\0')
// 					new->join_next = true;
// 				token_add_back(&data->token, new);
// 				ft_free((void **)&input->token);
// 			}
// 			printf("start = %d\ti = %d\n", start, i);
// 			printf("line[%d] = [%c]\n", i, input->line[i]);
// 			i = check_separators(input, start, i, data);
// 			if (i == -1)
// 				return (-1);
// 		}
// 	}
// 	return (0);
// }
