/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 12:10:02 by tcali             #+#    #+#             */
/*   Updated: 2025/09/24 16:18:36 by tcali            ###   ########.fr       */
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
	if (!ft_strncmp(str, "&&", 3))
		return (AND);
	if (!ft_strncmp(str, "||", 3))
		return (OR);
	return (CMD);
}

t_token	*new_token(char *str, bool join_next)
{
	t_token	*node;

	node = safe_malloc(sizeof(t_token));
	node->str = ft_strdup(str);
	node->pos = 0;
	node->type = get_token_type(str);
	node->input = -1;
	node->output = -1;
	node->to_expand = false;
	node->join_next = join_next;
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

// // creates a new token, adds it back to the list and defines its type.
// // also counting nb of cmds.
// int	create_add_token(t_data *data)
// {
// 	int		i;
// 	t_token	*new;

// 	i = 0;
// 	while (data->tokens[i])
// 	{
// 		new = new_token(data->tokens[i]);
// 		if (!new)
// 			return (0);
// 		token_add_back(&data->token, new);
// 		i++;
// 	}
// 	free_array(&data->tokens);
// 	data->array_alloc = false;
// 	if (check_type(data->token, data) == 0)
// 		return (0);
// 	print_tokens(data);
// 	data->nb_cmds = count_cmds(data->token);
// 	return (1);
// }

// joins a CMD and an ARG into one single CMD token.
// updates token->position.
t_token	*add_arg(t_token *current, t_data *data)
{
	t_token	*new_next;
	char	*tmp;

	(void)data;
	tmp = NULL;
	new_next = NULL;
	if (!current || !current->str || !current->next || !current->next->str)
		return (NULL);
	if (current->next->next)
	{
		new_next = current->next->next;
		new_next->prev = current;
		update_position(new_next);
	}
	// print_current_token(current);
	tmp = ft_strdup(current->str);
	ft_free((void **)&current->str);
	if (current->join_next)
		current->str = ft_strjoin(tmp, current->next->str);
	else
		current->str = ft_str_threejoin(tmp, " ", current->next->str);
	ft_free((void **)&tmp);
	current->join_next = current->next->join_next;
	ft_free((void **)&current->next->str);
	ft_free((void **)&current->next);
	current->next = new_next;
	return (current);
}
