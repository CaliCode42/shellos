/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 11:12:47 by tcali             #+#    #+#             */
/*   Updated: 2025/09/24 19:31:54 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**array_dup(char **array)
{
	int		i;
	char	**copy;

	i = 0;
	while (array[i])
		i++;
	copy = safe_malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (array[i])
	{
		copy[i] = ft_strdup(array[i]);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

t_env	*copy_node(t_env *src)
{
	t_env	*node;

	node = safe_malloc(sizeof(t_env));
	node->exported = src->exported;
	node->variable = ft_strdup(src->variable);
	node->value = NULL;
	if (src->value)
	{
		node->value = ft_strdup(src->value);
	}
	node->prev = NULL;
	node->next = NULL;
	return (node);
}

t_env	*copy_export(t_env **exported)
{
	t_env	*node;
	t_env	*new;
	t_env	*current;

	node = NULL;
	new = NULL;
	current = *exported;
	while (current)
	{
		// print_current_export(current);
		node = copy_node(current);
		export_add_back(&new, node);
		current = current->next;
	}
	sort_export(new);
	return (new);
}

void	print_export(t_env *exported)
{
	t_env	*current;
	t_env	*tmp;

	tmp = copy_export(&exported);
	current = tmp;
	while (current)
	{
		if (current->exported == true)
			safe_print("export ", false, false);
		safe_print(current->variable, false, false);
		if (current->value)
		{
			safe_print("=\"", false, false);
			safe_print(ft_strjoin(current->value, "\""), true, false);
		}
		safe_print("\n", false, false);
		current = current->next;
	}
	free_exported(tmp);
}

int	is_valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (str[0] != '_' && !ft_isalpha(str[0]))
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (str[i] != '_' && !ft_isalnum(str[i]))
		{
			if (str[i] == '+' && str[i + 1] == '=')
				i++;
			else
				return (0);
		}
		i++;
	}
	if (str[i] == '=')
	{
		while (str[++i])
		{
			if (ft_strchr("&|", str[i]))
				return (0);
		}
	}
	return (1);
}

int	is_redirection(t_tkn_code type)
{
	return (type == IN || type == OUT || type == APPEND || type == HEREDOC);
}

void	print_error(char *error, t_print_code code, t_token *token)
{
	rl_on_new_line();
	rl_replace_line("", 0);
	if (code == OFF)
		printf("%s\n", error);
	if (code == PRE)
		printf("%s : %s\n", token->str, error);
	if (code == POST)
		printf("%s : %s\n", error, token->str);
}

void	safe_print(char *str, bool free, bool newline)
{
	rl_on_new_line();
	rl_replace_line("", 0);
	printf("%s", str);
	if (newline)
		printf("\n");
	if (free)
		ft_free((void **)&str);
}
