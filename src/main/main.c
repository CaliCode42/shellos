/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 18:29:44 by tcali             #+#    #+#             */
/*   Updated: 2025/09/19 16:53:55 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_exit_status = 0;

// // get_line modified for tester (don't print the prompt)
// void	get_line(t_data *data, char **envp, char *line)
// {
// 	while (1)
// 	{
// 		if (isatty(STDIN_FILENO))
// 			line = readline(PROMPT);
// 		else
// 			line = readline(NULL);
// 
// 		if (!line)
// 		{
// 			if (isatty(STDIN_FILENO))
// 				printf("exit\n");
// 			free_minishell(data, true);
// 			break ;
// 		}
// 		if (*line)
// 			add_history(line);
// 		data->tokens = parse_line(line);
// 		if (!data->tokens)
// 		{
// 			printf("Error parsing line\n");
// 			break ;
// 		}
// 		init_data(data, envp);
// 		fork_process(data);
// 		free_minishell(data, false);
// 		free(line);
// 		data->token = NULL;
// 	}
// }
// 
// function which calls readline() and parse its result. Also initializing
// data before proceeding to execution of the cmds. 
// free what needs to be after each iteration and free all at the end.
void	get_line(t_data *data, char **envp, t_input *input)
{
	while (1)
	{
		// printf("g_exit_status = %d\n", g_exit_status);
		input->line = readline(PROMPT);
		if (!input->line)
		{
			safe_print("exit", false, true);
			free_minishell(data, true);
			break ;
		}
		if (*(input)->line)
			add_history(input->line);
		if (data->env_set == false)
			init_env(data, envp);
		else if (data->env_set && !data->envp)
			export_to_env(data);
		// data->tokens = parse_line(line);
		if (parse_line(data, input) != -1)
		{
			if (init_data(data) == 1)
				fork_process(data);
		}
		// if (!data->tokens)
		// {
		// 	safe_print("Error parsing line", false, true);
		// 	break ;
		// }
		free_minishell(data, false);
		ft_free((void **)&input->line);
	}
}

void	init_input(t_input *input)
{
	input->line = NULL;
	input->new_line = NULL;
	input->quote = '"';
	input->quoted = NULL;
	input->quotes = false;
	input->remaining = NULL;
	input->token = NULL;
}

// main function, called with envp as a parameter to be able to access
// to env variables. Setting signals before calling get_line(), which will
// call readline.
// reset_colors called at end of prgm.
int	main(int ac, char **av, char **envp)
{
	t_data	data;
	t_input	input;
	// char	*line;

	(void)ac;
	(void)av;
	// input = safe_malloc(sizeof(t_input));
	ft_bzero(&data, sizeof(t_data));
	// line = NULL;
	init_input(&input);
	// signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	get_line(&data, envp, &input);
	reset_colors();
	return (0);
}
