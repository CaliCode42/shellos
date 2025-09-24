/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 12:10:17 by tcali             #+#    #+#             */
/*   Updated: 2025/09/15 16:58:02 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	no_fork_needed(char *cmd)
{
	if (!cmd)
		return (0);
	else if (!ft_strncmp(cmd, "cd", 3)
		|| !ft_strncmp(cmd, "exit", 5)
		|| !ft_strncmp(cmd, "export", 7)
		|| !ft_strncmp(cmd, "unset", 6)
		|| !ft_strncmp(cmd, "pwd", 4))
		return (1);
	else
		return (0);
}

t_token	*skip_to_next(t_token *token, t_tkn_code type)
{
	t_token	*current;

	if (!token)
		return (NULL);
	current = token->next;
	if (type < 0)
		return (current);
	while (current)
	{
		if (current->type == type)
			return (current);
		current = current->next;
	}
	return (current);
}

void	update_position(t_token *token)
{
	t_token	*current;

	current = token;
	while (current)
	{
		current->pos -= 1;
		current = current->next;
	}
}

int	count_cmds(t_token *token)
{
	t_token	*current;
	int		nb_cmds;

	current = token;
	nb_cmds = 0;
	while (current)
	{
		if (current->type == CMD)
			nb_cmds++;
		current = current->next;
	}
	return (nb_cmds);
}

t_token	*find_prev_cmd(t_token *token)
{
	t_token	*current;

	current = token;
	while (current)
	{
		if (current->type == CMD)
			return (current);
		current = current->prev;
	}
	return (NULL);
}
