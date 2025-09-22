/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 17:32:40 by tcali             #+#    #+#             */
/*   Updated: 2025/09/19 16:30:16 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	g_exit_status = 130;
	// write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	// printf("test\n");
	write(1, "\n", 1);
	// if (g_exit_status != 0)
		rl_redisplay();
}


void	setup_heredoc_signals(void)
{
	signal(SIGINT, SIG_IGN);
}

void	check_sigint_sigquit(int status, t_data *data)
{
	int	sig;

	// printf("test\n");
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
		{
			g_exit_status = 131;
			data->last_exit = 131;
			// free_minishell(data, true);
			safe_print("Quit (Core dumped)", false, true);
		}
		else if (sig == SIGINT)
		{
			g_exit_status = 130;
			data->last_exit = 130;
			// free_minishell(data, false);
			write(1, "\n", 1);
			rl_replace_line("", 0);
			rl_on_new_line();
			// handle_sigint(sig);
		}
		else
			write(1, "\n", 1);
	}
}
