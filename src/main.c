/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 18:29:44 by tcali             #+#    #+#             */
/*   Updated: 2025/06/23 12:34:00 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_line(t_data *data, char **envp, char *line)
{
	while (1)
	{
		line = readline(PROMPT);
		if (!line)
		{
			printf("exit\n");
			free_minishell(data);
			break ;
		}
		if (*line)
			add_history(line);
		data->tokens = parse_line(line);
		if (!data->tokens)
		{
			printf("Error parsing line\n");
			break ;
		}
		init_data(data, envp);
		fork_process(data);
		free(line);
		free_list(&data->token);
		data->token = NULL;
		data->nb_pipes = 0;
	}
}

int	main(int ac, char **av, char **envp)
{
	t_data	data;
	char	*line;

	(void)ac;
	(void)av;
	line = NULL;
	data.tokens = NULL;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	get_line(&data, envp, line);
	reset_colors();
	return (0);
}
