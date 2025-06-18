/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 18:29:44 by tcali             #+#    #+#             */
/*   Updated: 2025/06/18 13:42:00 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child(t_data *data)
{
	//signal(SIGQUIT, SIG_DFL);
	//signal(SIGINT, SIG_DFL);
	execute_command(data->token->str, data->envp);
	exit(1);
}

void	parent(t_data *data)
{
	int	status;
	int	sig;

	waitpid(data->pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
			printf("Quit (Core dumped)\n");
		else if (sig == SIGINT)
			printf("\n");
	}
}

void	check_ctrl_slash(t_token *current)
{
	t_token	*new_next;

	new_next = NULL;
	if (current->next && current->next->str)
	{
		if (current->next->next)
			new_next = current->next->next;
		current->str = ft_str_threejoin(current->str, " ", current->next->str);
		free(current->next->str);
		free(current->next);
		current->next = new_next;
	}
}

void	fork_process(t_data *data)
{
	t_token	*current;

	current = data->token;
	while (current)
	{
		if (current->type == CMD)
		{
			if (!ft_strncmp(current->str, "sleep", 5)) // just to check for ctrl + /
				check_ctrl_slash(current);
			if (is_builtin(current->str))
				exec_builtin(current, data);
			else
			{
				printf("not a builtin\n");
				data->pid = fork();
				if (data->pid == 0)
					child(data);
				else if (data->pid > 0)
					parent(data);
				else
					perror("fork");
			}
		}
		current = current->next;
	}
}

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
