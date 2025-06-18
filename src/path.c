/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 13:22:04 by tcali             #+#    #+#             */
/*   Updated: 2025/06/18 15:26:20 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                                                                            */
/* 	  		fcts to find the PATH of a command and check its validity		  */
/*                                                                            */
/* ************************************************************************** */
char	*find_cmd_path(char *cmd, char **path)
{
	char	*cmd_path;
	int		i;

	i = 0;
	while (path[i])
	{
		cmd_path = ft_str_threejoin(path[i], "/", cmd);
		if (!cmd_path)
			return (NULL);
		if (access(cmd_path, F_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	return (NULL);
}

char	*get_cmd_path(char *cmd, char **env)
{
	char	**path;
	char	*bin;
	int		i;

	if (cmd[0] == '/')
	{
		if (access(cmd, F_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	i = 0;
	while (ft_strnstr(env[i], "PATH=", 5) == 0)
		i++;
	path = ft_split(env[i] + 5, ':');
	bin = find_cmd_path(cmd, path);
	if (!bin)
	{
		free_array(path);
		return (NULL);
	}
	return (bin);
}

/* ************************************************************************** */
/*                                                                            */
/* 	  		fcts to identify the aim of a cd cmd and check its validity		  */
/*                                                                            */
/* ************************************************************************** */
//
/*
Checks if arg is valid : returns 0 if OK, else returns 1.
*/

int	is_valid_dir(char *path)
{
	struct stat	sb;

	if (!path)
		return (0);
	if (!ft_strncmp(path, "..", 3) || !ft_strncmp(path, ".", 2))
		return (1);
	if (stat(path, &sb) == -1)
	{
		perror("cd: No such file or directory");
		return (0);
	}
	if (!S_ISDIR(sb.st_mode))
	{
		ft_printf_fd(STDERR_FILENO, "cd: %s: Not a directory\n", path);
		return (0);
	}
	if (access(path, X_OK) != 0)
	{
		perror("cd: Permission denied");
		return (0);
	}
	return (1);
}

void	go_to(char *path)
{
	if (chdir(path) == -1)
		perror("chdir failed\n");
}
