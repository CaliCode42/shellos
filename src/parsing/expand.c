/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:31:20 by chdoe             #+#    #+#             */
/*   Updated: 2025/09/19 17:58:05 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// static int	is_stop_exp(char c)
// {
// 	int			i;
// 	static char	*check;

// 	i = 0;
// 	check = " \"$'[]%=/><\0";
// 	while (i != 13)
// 	{
// 		if (check[i] == c || (c >= 97 && c <= 122))
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }

// static size_t	len_var(size_t i, char *line, char **env, size_t start)
// {
// 	size_t	j;
// 	char	*exp;
// 	char	*exp_first;
// 	size_t	len_exp;

// 	j = 0;
// 	len_exp = 0;
// 	while (line[i + j] && line[i + j] != '$' && is_stop_exp(line[i + j])
// 		&& i + j < ft_strlen(line))
// 		j++;
// 	exp_first = ft_substr(line, start, j);
// 	if (!exp_first)
// 		return (-1);
// 	exp = ft_strjoin(exp_first, "=");
// 	free (exp_first);
// 	if (!exp)
// 		return (-1);
// 	j = 0;
// 	// print_array(env, true);
// 	while (env[j] && line[i])
// 	{
// 		if (ft_strnstr(env[j], exp, ft_strlen(exp)))
// 			len_exp = len_exp + (ft_strlen(env[j]) - (ft_strlen(exp) * 2));
// 		j++;
// 	}
// 	free(exp);
// 	return (len_exp);
// }

// static size_t	ft_len_expand(char *line, char **env)
// {
// 	size_t	i;
// 	size_t	len_exp;
// 	size_t	start;

// 	i = 0;
// 	if (!line)
// 		return (0);
// 	len_exp = ft_strlen(line);
// 	while (i < ft_strlen(line))
// 	{
// 		while (line[i] && line[i] != '$')
// 			i++;
// 		if (line[i])
// 			i++;
// 		start = i;
// 		while (line[i] && !is_stop_exp(line[i]))
// 			i++;
// 		len_exp += (len_var(i, line, env, start));
// 		i++;
// 	}
// 	printf("len_exp = %ld\n", len_exp);
// 	return (len_exp);
// }

// static size_t	expand_var(char *line, char *exp, char **env)
// {
// 	size_t		i;
// 	size_t		j;
// 	size_t		k;
// 	char		*exp_var;
// 	char		*exp_first;

// 	i = 0;
// 	k = 0;
// 	while (line[i] && is_stop_exp(line[i]))
// 		i++;
// 	exp_first = ft_substr(line, 0, i);
// 	if (!exp_first)
// 		return (-1);
// 	exp_var = ft_strjoin(exp_first, "=");
// 	free(exp_first);
// 	if (!exp_var)
// 		return (-1);
// 	j = 0;
// 	while (env[j])
// 	{
// 		if (ft_strnstr(env[j], exp_var, ft_strlen(exp_var)))
// 			break ;
// 		j++;
// 	}
// 	if (!env[j])
// 	{
// 		free (exp_var);
// 		return (0);
// 	}
// 	i++;
// 	while (env[j][i])
// 	{
// 		exp[k] = env[j][i];
// 		i++;
// 		k++;
// 	}
// 	free(exp_var);
// 	return (k);
// }

// char	*expand_quotes(char *line, char **env)
// {
// 	size_t	i;
// 	size_t	j;
// 	size_t	len_exp;
// 	char	*exp;

// 	i = 0;
// 	j = 0;
// 	// print_array(env, true);
// 	len_exp = ft_len_expand(line, env);
// 	exp = NULL;
// 	if (len_exp)
// 		exp = safe_malloc(sizeof(char) * (len_exp + 1));
// 	while (line[i])
// 	{
// 		if (line[i] != '$')
// 		{
// 			exp[j] = line[i];
// 			printf("exp[%ld] = %c\n", j, exp[j]);
// 			i++;
// 			j++;
// 		}
// 		else
// 			i++;
// 		j += expand_var(&line[i], &exp[j], env);
// 		while (line[i] && is_stop_exp(line[i]))
// 			i++;
// 	}
// 	exp[len_exp] = '\0';
// 	printf("line = [%s]\n", line);
// 	printf("exp = [%s]\n", exp);
// 	if (exp && *exp)
// 		return (exp);
// 	return (ft_free((void **)&exp), NULL);
// }

void	token_add_next(t_token *current, t_token *new)
{
	new->type = ARG;
	// printf("\nnew");
	// print_current_token(new);
	if (current->next)
		current->next->prev = new;
	new->prev = current;
	new->next = current->next;
	current->next = new;
	update_position(new);
}

char	*var_to_value(char *exp, t_token *current)
{
	int		i;
	int		j;
	char	*var;
	char	*remaining;

	i = 1;
	j = 0;
	while (exp[i] && ft_isalnum(exp[i]))
		i++;
	var = safe_malloc(sizeof(char) * i + 1);
	remaining = safe_malloc(sizeof(char) * (ft_strlen(exp) - i + 1));
	i = 1;
	while (exp[i] && ft_isalnum(exp[i]))
	{
		var[i - 1] = exp[i];
		i++;
	}
	var[i - 1] = '\0';
	while (exp[i])
	{
		remaining[j++] = exp[i];
		i++;
	}
	remaining[j] = '\0';
	// printf("remaining = [%s]\n", remaining);
	if (remaining)
	{
		token_add_next(current, new_token(remaining));
		if (remaining[0] == '$')
			current->next->to_expand = true;
	}
	ft_free((void **)&remaining);
	current->next->join_next = true;
	// printf("\nnew");
	// print_current_token(current->next);
	// printf("\nvar = [%s]\n", var);
	return (var);
}


char	*expand_quotes(char *exp, t_data *data, t_token *token)
{
	t_env	*current;
	char	*var;

	// printf("token = [%s]\n", exp);
	token->type = ARG;
	if (ft_strchr(exp, '\''))
	{
		token->to_expand = false;
		return (var_to_value(exp, token));
	}
	if (exp[1] == '?')
	{
		var = ft_itoa(data->last_exit);
		// printf("new token = [%s]\n", var);
		return (var);
	}
	else if (!ft_strncmp(exp, "~", 2))
		var = ft_strdup("HOME");
	else
	{
		var = var_to_value(exp, token);
	}
	current = data->exported_env;
	while (current)
	{
		if (!ft_strncmp(current->variable, var, ft_strlen(var))
			&& current->variable[ft_strlen(var)] == '\0')
			return (ft_free((void **)&var), ft_strdup(current->value));
		current = current->next;
	}
	// printf("new token = [%s]\n", var);
	if (!ft_strncmp(exp, "~", 2))
		return (ft_free((void **)&var), ft_strdup(exp));
	return (ft_free((void **)&var), NULL);
}
