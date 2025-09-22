/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 16:40:32 by tcali             #+#    #+#             */
/*   Updated: 2025/09/15 14:31:01 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function to copy the array** env to a chained list of t_env.
void	env_to_export(char **envp, t_env **exported)
{
	t_env	*new;
	int		i;

	i = 0;
	new = NULL;
	while (envp[i])
	{
		new = new_exported(envp[i]);
		export_add_back(exported, new);
		i++;
	}
}

// swapping the content of t_env *current && *next.
bool	swap_export(t_env *current, bool swapped)
{
	char	*tmp_var;
	char	*tmp_val;
	bool	tmp_exported;

	tmp_var = current->variable;
	current->variable = current->next->variable;
	current->next->variable = tmp_var;
	tmp_val = current->value;
	current->value = current->next->value;
	current->next->value = tmp_val;
	tmp_exported = current->exported;
	current->exported = current->next->exported;
	current->next->exported = tmp_exported;
	swapped = true;
	return (swapped);
}

// sorts t_env *exported by alphabetical order.
void	sort_export(t_env *exported)
{
	bool	swapped;
	t_env	*current;

	if (!exported)
		return ;
	swapped = true;
	while (swapped)
	{
		swapped = false;
		current = exported;
		while (current->next)
		{
			if (ft_strncmp(current->variable,
					current->next->variable, INT_MAX) > 0)
				swapped = swap_export(current, swapped);
			current = current->next;
		}
	}
}

// updates an existing exported var.
void	update_export(t_env *current, t_env *new, char *value, bool append)
{
	char	*new_value;

	new_value = NULL;
	if (append == true && current->value)
	{
		new_value = ft_strjoin(current->value, value);
		ft_free((void **)&current->value);
		ft_free((void **)&value);
		current->value = ft_strdup(new_value);
		ft_free((void **)&new_value);
	}
	else
	{
		if (current->value)
			ft_free((void **)&current->value);
		current->value = ft_strdup(value);
		ft_free((void **)&value);
	}
	if (new)
	{
		ft_free((void **)&new->variable);
		ft_free((void **)&new);
	}
}

// checks presence of '+' before '=' to see if the value should be
// added or if it should replace the existing one.
// returns (true) or (false).
bool	is_append(t_env *new, int len_variable)
{
	bool	append;
	char	*var;

	append = false;
	if (ft_strchr(new->variable, '+'))
	{
		len_variable -= 1;
		var = ft_strndup(new->variable, len_variable);
		if (!var)
			error_exit("failed to update export.\n");
		ft_free((void **)&new->variable);
		new->variable = var;
		append = true;
	}
	return (append);
}
