/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 12:10:02 by tcali             #+#    #+#             */
/*   Updated: 2025/06/23 13:49:57 by tcali            ###   ########.fr       */
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
	return (CMD);
}

t_token	*new_token(char *str, int i)
{
	t_token	*node;

	node = safe_malloc(sizeof(t_token));
	node->str = ft_strdup(str);
	node->pos = i;
	node->type = get_token_type(str);
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
}

void	create_add_token(t_data *data)
{
	int		i;
	t_token	*new;

	i = 0;
	while (data->tokens[i])
	{
		new = new_token(data->tokens[i], i);
		printf("creating token[%d] = %s\n", i, data->tokens[i]);
		token_add_back(&data->token, new);
		i++;
	}
	free_array(data->tokens, 0);
	data->array_alloc = false;
	check_type(data->token);
	//print_token(data);
}

void	add_arg(t_token *current)
{
	t_token	*new_next;
	char	*tmp;

	new_next = NULL;
	if (!current->next || !current->next->str)
		return ;
	if (current->next->next)
		new_next = current->next->next;
	tmp = current->str;
	current->str = ft_str_threejoin(current->str, " ", current->next->str);
	free(tmp);
	free(current->next->str);
	free(current->next);
	current->next = new_next;
	printf("cmd = %s\n", current->str);
}
