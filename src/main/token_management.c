/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 12:10:02 by tcali             #+#    #+#             */
/*   Updated: 2025/09/22 12:18:31 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tkn_code	get_token_type(char *str)
{
	if (!ft_strncmp(str, "|", 2))
		return (PIPE);
	if (!ft_strncmp(str, "<", 2))
		return (IN);
	if (!ft_strncmp(str, ">", 2))
		return (OUT);
	if (!ft_strncmp(str, ">>", 3))
		return (APPEND);
	if (!ft_strncmp(str, "<<", 3))
		return (HEREDOC);
	if (!ft_strncmp(str, "||", 3))
		return (OR);
	if (ft_strchr(str, '&'))
	{
		if (!ft_strncmp(str, "&&", 3))
			return (AND);
		if (syntax_error(str))
		{
			safe_print("bash: syntax error near unexpected token `&'",
				false, true);
			return (ft_free((void **)&str), (t_tkn_code)-1);
		}
		return (CMD);
	}
	return (CMD);
}

t_token	*new_token(char *str)
{
	t_token	*node;

	node = safe_malloc(sizeof(t_token));
	node->str = ft_strdup(str);
	node->pos = 0;
	node->type = get_token_type(str);
	if (node->type == (t_tkn_code)-1)
		return (ft_free((void **)&node), NULL);
	node->input = -1;
	node->output = -1;
	node->to_expand = false;
	node->join_next = false;
	node->to_split = true;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

void	token_add_back(t_token **lst, t_token *new)
{
	t_token	*current;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	current = *lst;
	while (current->next)
		current = current->next;
	current->next = new;
	new->prev = current;
	new->pos = current->pos + 1;
}

// creates a new token, adds it back to the list and defines its type.
// also counting nb of cmds.
int	create_add_token(t_data *data)
{
	int		i;
	t_token	*new;

	i = 0;
	while (data->tokens[i])
	{
		new = new_token(data->tokens[i]);
		if (!new)
			return (0);
		token_add_back(&data->token, new);
		i++;
	}
	free_array(&data->tokens);
	data->array_alloc = false;
	if (check_type(data->token, data) == 0)
		return (0);
	print_tokens(data);
	data->nb_cmds = count_cmds(data->token);
	return (1);
}

// joins a CMD and an ARG into one single CMD token.
// updates token->position.
t_token	*add_arg(t_token *current, t_data *data)
{
	t_token	*new_next;
	char	*tmp;
	char	*arg;

	new_next = NULL;
	if (!current || !current->str || !current->next || !current->next->str)
		return (NULL);
	if (current->next->next)
	{
		new_next = current->next->next;
		new_next->prev = current;
		update_position(new_next);
	}
	tmp = ft_strdup(current->str);
	if (current->next->to_expand == true)
		arg = expand_quotes(current->next->str, data, current->next);
	else
		arg = current->next->str;
	if (!arg)
	{
		ft_free((void **)&tmp);
		ft_free((void **)&current->next->str);
		ft_free((void **)&current->next);
		current->next = new_next;
		return (current);
	}
	ft_free((void **)&current->str);
	if (current->join_next)
		current->str = ft_strjoin(tmp, arg);
	else
		current->str = ft_str_threejoin(tmp, " ", arg);
	if (!current->next->join_next)
		current->join_next = false;
	ft_free((void **)&tmp);
	ft_free((void **)&current->next->str);
	ft_free((void **)&current->next);
	current->next = new_next;
	return (current);
}
