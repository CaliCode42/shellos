/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 13:28:45 by tcali             #+#    #+#             */
/*   Updated: 2025/09/18 15:12:58 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_current_token(t_token *current)
{
	char	*type_str = "UNKNOWN";

	if (!current)
	{
		printf("current = NULL\n");
		return ;
	}
	if (!current->str)
	{
		printf("current->str = NULL\n");
		return ;
	}
	if (current->type == CMD)
		type_str = "CMD";
	else if (current->type == ARG)
		type_str = "ARG";
	else if (current->type == IN)
		type_str = "INPUT";
	else if (current->type == OUT)
		type_str = "OUTPUT";
	else if (current->type == APPEND)
		type_str = "APPEND";
	else if (current->type == HEREDOC)
		type_str = "HEREDOC";
	else if (current->type == PIPE)
		type_str = "PIPE";
	else if (current->type == AND)
		type_str = "AND";
	else if (current->type == OR)
		type_str = "OR";
	printf("\n--------------\n");
	printf("token : %s\n", current->str);
	printf("position      : %d\n", current->pos);
	printf("type          : %s\n", type_str);
	printf("input          : %d\n", current->input);
	printf("output          : %d\n", current->output);
	if (current->to_expand == true)
		printf("to_expand        : true\n");
	else
		printf("to_expand        : false\n");
	if (current->join_next == true)
		printf("join_next        : true\n");
	else
		printf("join_next        : false\n");
	if (current->prev && current->prev->str)
		printf("prev          : %s\n", current->prev->str);
	else
		printf("prev          : NULL\n");
	if (current->next && current->next->str)
		printf("next          : %s\n", current->next->str);
	else
		printf("next          : NULL\n");
	printf("--------------\n");
}

void	print_tokens(t_data *data)
{
	t_token	*current;

	current = data->token;
	printf("print_tokens :\n\n");
	while (current)
	{
		print_current_token(current);
		current = current->next;
	}
	printf("\n--------------------------------------\n");
}

void	reset_colors(void)
{
	printf("%s", C_RST);
}

void	print_array(char **array, bool debug)
{
	int	i;

	i = 0;
	if (debug == true)
	{
		while (array[i])
		{
			printf("array[%d] = [%s]\n", i, array[i]);
			i++;
		}
	}
	else
	{
		while (array[i])
		{
			safe_print(array[i], false, true);
			i++;
		}
	}
}

void	print_current_export(t_env *current)
{
	printf("\n\t**********\n");
	printf("variable = [%s]\n", current->variable);
	printf("value = [%s]\n", current->value);
	if (current->exported == true)
		printf("exported = true\n");
	else
		printf("exported = false\n");
	if (!current->prev)
		printf("prev = NULL\n");
	else
		printf("prev = %s\n", current->prev->variable);
	if (!current->next)
		printf("next = NULL\n");
	else
		printf("next = %s\n", current->next->variable);
}
