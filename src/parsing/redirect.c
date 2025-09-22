/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:35:24 by chdoe             #+#    #+#             */
/*   Updated: 2025/09/17 12:23:42 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parsing.h"


static int	triple_in(char *line)
{
	char	*check;

	check = ft_strnstr(line, "<<<", ft_strlen(line));
	if (!check)
		return (1);
	return (0);
}

static int	triple_out(char *line)
{
	char	*check;

	check = ft_strnstr(line, ">>>", ft_strlen(line));
	if (!check)
		return (1);
	return (0);
}

int	triple_check(char *line, char c)
{
	char	*check;
	char	triple[3];

	triple[0] = c;
	triple[1] = c;
	triple[2] = c;
	triple[3] = '\0';
	check = ft_strnstr(line, triple, ft_strlen(line));
	if (!check)
		return (1);
	return (0);
}

int	redirect_inout(char *line)
{
	int	i;

	i = 0;
	if (!triple_check(line, '<'))
		return (-1);
	if (!triple_check(line, '>'))
		return (-1);
	while (line[i])
	{
		if (line[i] && (line[i] == '<' || line[i] == '>'))
		{
			i++;
			while (line[i] && ft_isspace(line[i]))
				i++;
			if (is_bad_redirect(line[i]))
				return (-1);
		}
		else
			i++;
	}
	return (0);
}

int	append_inout(char *line)
{
	int	i;

	i = 0;
	if (!triple_in(line))
		return (-1);
	if (!triple_out(line))
		return (-1);
	while (line[i])
	{
		if (line[i + 1]
			&& ((line[i] == '<' && line[i + 1] == '<')
				|| (line[i] == '>' && line[i + 1] == '>')))
		{
			i += 2;
			while (line[i] && ft_isspace(line[i]))
				i++;
			if (is_bad_redirect(line[i]))
				return (-1);
		}
		else
			i++;
	}
	return (0);
}