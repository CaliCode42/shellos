/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 13:28:45 by tcali             #+#    #+#             */
/*   Updated: 2025/06/23 17:42:44 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_token(t_data *data)
{
	t_token	*current;
	char	*str;

	current = data->token;
	str = NULL;
	while (current)
	{
		if (current->type == CMD)
			str = "cmd";
		else if (current->type == ARG)
			str = "arg";
		else if (current->type == IN)
			str = "input";
		else if (current->type == OUT)
			str = "output";
		else if (current->type == APPEND)
			str = "append";
		else if (current->type == HEREDOC)
			str = "heredoc";
		else if (current->type == PIPE)
			str = "pipe";
		printf("current token : %s\nposition : %d\ntype : %s\n",
			current->str, current->pos, str);
		current = current->next;
	}
}

void	reset_colors(void)
{
	printf("%s", C_RST);
}
