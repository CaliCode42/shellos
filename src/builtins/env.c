/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 15:20:01 by tcali             #+#    #+#             */
/*   Updated: 2025/09/15 14:46:12 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export_to_env(t_data *data)
{
	int		i;
	t_env	*current;
	char	*var;

	i = 0;
	if (!data->exported_env)
		return ;
	current = data->exported_env;
	// print_export(data->exported_env);
	while (current)
	{
		if (current->value)
			var = ft_str_threejoin(current->variable, "=", current->value);
		else
			var = ft_strjoin(current->variable, "=");
		add_to_env(data, var);
		free(var);
		i++;
		current = current->next;
	}
	data->env_set = true;
}

t_env	*find_env(t_data *data, char *var)
{
	t_env	*current;
	int		len;

	current = data->exported_env;
	len = ft_strlen(var);
	while (current)
	{
		if (!ft_strncmp(current->variable, var, len))
		{
			return (current);
		}
		current = current->next;
	}
	return (NULL);
}

int	builtin_env(t_data *data)
{
	print_array(data->envp, false);
	return (0);
}

void	add_to_env(t_data *data, char *var)
{
	int		i;
	char	**new_env;

	i = 0;
	if (!data->envp)
	{
		new_env = safe_malloc(sizeof(char *) * 2);
		new_env[i++] = ft_strdup(var);
		new_env[i] = NULL;
		data->envp = new_env;
		return ;
	}
	while (data->envp[i])
		i++;
	new_env = safe_malloc(sizeof(char *) * (i + 2));
	i = 0;
	while (data->envp[i])
	{
		new_env[i] = ft_strdup(data->envp[i]);
		i++;
	}
	new_env[i++] = ft_strdup(var);
	new_env[i] = NULL;
	free_array(&data->envp);
	data->envp = new_env;
}
