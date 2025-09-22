/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:17:49 by tcali             #+#    #+#             */
/*   Updated: 2025/09/15 14:47:07 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*parse_export(t_token *token)
{
	t_env	*tmp;
	char	**identifier;

	if (!is_valid_identifier(token->str))
	{
		safe_print(ft_strjoin("export : ", token->str), true, false);
		safe_print(" : not a valid identifier.", false, true);
		exit (1);
	}
	tmp = safe_malloc(sizeof(t_env));
	tmp->exported = true;
	if (ft_strchr(token->str, '='))
	{
		identifier = ft_split(token->str, '=');
		tmp->variable = ft_strdup(identifier[0]);
		tmp->value = ft_strdup(identifier[1]);
		free_array(&identifier);
	}
	else
	{
		tmp->value = NULL;
		tmp->variable = ft_strdup(token->str);
	}
	tmp->prev = NULL;
	tmp->next = NULL;
	return (tmp);
}

// creates a new exported variable.
// checks presence of '=',
// to see if node->value should be set to a value or NULL.
t_env	*new_exported(char *var)
{
	t_env	*node;
	char	*equal;

	node = safe_malloc(sizeof(t_env));
	node->exported = true;
	equal = ft_strchr(var, '=');
	if (equal && ft_strlen(equal))
	{
		node->variable = ft_substr(var, 0, equal - var);
		node->value = ft_strdup(equal + 1);
	}
	else
	{
		node->variable = ft_strdup(var);
		node->value = NULL;
	}
	node->prev = NULL;
	node->next = NULL;
	return (node);
}

void	export_add_back(t_env **env, t_env *new)
{
	t_env	*current;

	if (!env || !new)
		return ;
	if (!*env)
	{
		*env = new;
		return ;
	}
	current = *env;
	while (current->next)
		current = current->next;
	current->next = new;
	new->prev = current;
}

void	add_or_update_export(t_data *data, t_env *new, int len_variable)
{
	bool	append;
	t_env	*current;

	data->env_set = true;
	append = is_append(new, len_variable);
	if (append == true)
		len_variable -= 1;
	current = data->exported_env;
	while (current)
	{
		if (!ft_strncmp(current->variable, new->variable, len_variable))
		{
			update_export(current, new, new->value, append);
			return ;
		}
		current = current->next;
	}
	new->exported = true;
	export_add_back(&data->exported_env, new);
	// print_export(data->exported_env);
}

int	builtin_export(t_data *data, t_token *token)
{
	t_token	*current;
	t_env	*new;
	int		len_variable;

	current = token->next;
	// printf("nb of pipes = %d\n", data->nb_pipes);
	if (!current || current->type != ARG)
		print_export(data->exported_env);
	else
	{
		while (current && current->type == ARG)
		{
			new = parse_export(current);
			// print_current_export(new);
			len_variable = ft_strlen(new->variable);
			add_or_update_export(data, new, len_variable);
			// ft_free((void **)&new->variable);
			// ft_free((void **)&new->value);
			current = current->next;
		}
		// print_export(data->exported_env);
	}
	return (0);
}
