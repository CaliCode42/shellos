/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 11:31:56 by tcali             #+#    #+#             */
/*   Updated: 2025/09/22 23:49:28 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// free the alloc'd data in prgm,
// using bool to define if it's end of prgm or not.
// when called at end of prgm, env is free and history is cleared.
void	free_minishell(t_data *data, bool end)
{
	if (!data)
		return ;
	free_heredoc(data);
	if (data->cmds)
		free_array(&data->cmds);
	free_list(&data->token);
	free_pids(data);
	free_pipes(data->pipe_fd, data->nb_pipes, data);
	free_array(&data->envp);
	if (end == true)
	{
		// printf("end\n");
		free_exported(data->exported_env);
		data->env_set = false;
		rl_clear_history();
	}
	// printf("giyugf  %d\n", data->last_exit);
}

// can be called with a specified size,
// if called with size = 0 it will free the array until array[i] = NULL.
void	free_array(char ***array)
{
	int	i;

	if (!array || !*array)
		return ;
	i = 0;
	while ((*array)[i])
	{
		ft_free((void **)&(*array)[i]);
		// array[i] = NULL;
		i++;
	}
	ft_free((void **)array);
}

// only for t_token.
void	free_list(t_token **lst)
{
	t_token	*tmp;

	while (*lst)
	{
		tmp = (*lst)->next;
		// if ((*lst)->str)
		// 	printf("free_list: freeing token [%s]\n", (*lst)->str);
		// else
		// 	printf("free_list: freeing empty token\n");
		ft_free((void **)&(*lst)->str);
		ft_free((void **)&(*lst));
		*lst = tmp;
	}
	// printf("free_list: done\n");
}

// // only for t_token.
// void	free_list(t_token **lst)
// {
// 	t_token	*tmp;
// 	t_token	*current;

// 	current = *lst;
// 	while (current)
// 	{
// 		tmp = current->next;
// 		ft_free((void **)&current->str);
// 		ft_free((void **)&current);
// 		current = tmp;
// 	}
// }

// free up to pipes[n - 1] and sets data->pipe_fd to 0;
// if data->nb_pipes = 0, nothing happends
void	free_pipes(int **pipes, int n, t_data *data)
{
	int	i;

	i = 0;
	if (!data->nb_pipes || !pipes)
		return ;
	while (i < n)
	{
		ft_free((void **)&pipes[i]);
		i++;
	}
	data->pipe_fd = 0;
	ft_free((void **)&pipes);
}

void	free_pids(t_data *data)
{
	if (!data->pids)
		return ;
	ft_free((void **)&data->pids);
}
