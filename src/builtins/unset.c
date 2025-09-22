/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 13:01:28 by tcali             #+#    #+#             */
/*   Updated: 2025/09/02 18:25:45 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*find_var(char *var, t_data *data)
{
	int		len_var;
	t_env	*current;

	len_var = ft_strlen(var);
	current = data->exported_env;
	while (current)
	{
		if (!ft_strncmp(var, current->variable, len_var))
			return (current);
		current = current->next;
	}
	return (NULL);
}

int	delete_from_export(char *var, t_data *data)
{
	t_env	*current;
	int		len_var;

	len_var = ft_strlen(var);
	current = data->exported_env;
	while (current)
	{
		if (!ft_strncmp(current->variable, var, len_var))
		{
			ft_free((void **)&current->variable);
			ft_free((void **)&current->value);
			if (current->prev)
				current->prev->next = current->next;
			else
				current->next->prev = NULL;
			if (current->next)
				current->next->prev = current->prev;
			else
				current->prev->next = NULL;
			ft_free((void **)&current);
			return (0);
		}
		current = current->next;
	}
	return (1);
}

// //duplicate data->envp without the var called with unset.
// deleted because useless !!! export should not add VAR to env, only to export.
// void	delete_var(char *found, t_data *data)
// {
// 	char	**new_env;
// 	int		i;
// 	int		j;

// 	printf("delete_var\n");
// 	i = 0;
// 	while (data->envp[i])
// 		i++;
// 	new_env = safe_malloc(sizeof(char *) * (i));
// 	i = 0;
// 	j = 0;
// 	while (data->envp[i])
// 	{
// 		if (data->envp[i] == found)
// 			i++;
// 		if (!data->envp[i])
// 			break ;
// 		new_env[j] = ft_strdup(data->envp[i]);
// 		j++;
// 		i++;
// 	}
// 	new_env[j] = NULL;
// 	free_array(data->envp, 0);
// 	data->envp = new_env;
// 	// print_array(data->envp, true);
// }

//if unset with no arg or invalid arg, just do nothing.
//
//if arg is valid, remove the corresponding VAR from env.
//----> browse through the args until finding correspondance.
//----> when found replace current env with a new one whitout mentionned VAR.
//----> if end of env reached and no correspondance, just return back to prgm
// (with success exit code).
int	builtin_unset(t_data *data, t_token *token)
{
	t_token	*current;
	t_env	*found;

	current = token->next;
	if (!current || current->type != ARG)
		return (0);
	while (current && current->type == ARG)
	{
		found = find_var(current->str, data);
		if (found != NULL)
			delete_from_export(current->str, data);
		current = current->next;
	}
	return (0);
}
