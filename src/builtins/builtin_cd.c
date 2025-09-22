/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 16:08:44 by tcali             #+#    #+#             */
/*   Updated: 2025/09/18 14:14:16 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	copy_env_except_oldpwd(t_data *data, char **new_env, char **old_pwd,
		char *cwd)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (data->envp[i])
	{
		if (!ft_strncmp("PWD=", data->envp[i], 4))
		{
			*old_pwd = ft_strjoin("OLD", data->envp[i]);
			ft_free((void **)&data->envp[i]);
			data->envp[i] = ft_strjoin("PWD=", cwd);
			i++;
		}
		else if (!ft_strncmp("OLDPWD=", data->envp[i], 7))
			i++;
		else
			new_env[j++] = ft_strdup(data->envp[i++]);
	}
	return (j);
}

char	*update_env_pwd(t_data *data, char *cwd)
{
	int		i;
	int		j;
	char	*old_pwd;
	char	**new_env;

	i = 0;
	while (data->envp[i])
		i++;
	new_env = safe_malloc(sizeof(char *) * (i + 1));
	j = copy_env_except_oldpwd(data, new_env, &old_pwd, cwd);
	free_array(&data->envp);
	if (j == i)
	{
		new_env[j] = NULL;
		data->envp = new_env;
		add_to_env(data, old_pwd);
	}
	else
	{
		new_env[j++] = ft_strdup(old_pwd);
		new_env[j] = NULL;
		data->envp = new_env;
	}
	return (old_pwd);
}

// char	*update_env_pwd(t_data *data, char *cwd)
// {
// 	int		i;
// 	int		j;
// 	char	*old_pwd;
// 	char	**new_env;

// 	i = 0;
// 	while (data->envp[i])
// 		i++;
// 	new_env = safe_malloc(sizeof(char *) * (i + 1));
// 	i = 0;
// 	j = 0;
// 	while (data->envp[i])
// 	{
// 		if (!ft_strncmp("PWD=", data->envp[i], 4))
// 			old_pwd = ft_strjoin("OLD", data->envp[i++]);
// 		else if (!ft_strncmp("OLDPWD=", data->envp[i], 7))
// 			i++;
// 		else
// 			new_env[j++] = ft_strdup(data->envp[i++]);
// 	}
// 	free_array(&data->envp);
// 	new_env[j++] = ft_strjoin("PWD=", cwd);
// 	// printf("i = %d\tj = %d\n", i, j);
// 	if (j == i)
// 	{
// 		new_env[j] = NULL;
// 		data->envp = new_env;
// 		add_to_env(data, old_pwd);
// 	}
// 	else
// 	{
// 		new_env[j++] = ft_strdup(old_pwd);
// 		new_env[j] = NULL;
// 		data->envp = new_env;
// 	}
// 	// print_array(data->envp, true);
// 	return (old_pwd);
// }

static void	update_pwd(t_data *data)
{
	char	*cwd;
	char	*old_pwd;
	char	*new_env;
	t_env	*new;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd\n");
		return ;
	}
	new_env = ft_strjoin("PWD=", cwd);
	old_pwd = update_env_pwd(data, cwd);
	new = new_exported(new_env);
	add_or_update_export(data, new, 3);
	ft_free((void **)&new_env);
	new = new_exported(old_pwd);
	add_or_update_export(data, new, 6);
	ft_free((void **)&old_pwd);
	ft_free((void **)&cwd);
}

/*********************************************************************/
/*
	if arg == ".." >> close current directory and get back to previous
	if arg is a valid path and access is authorized, go to directory
	if no arg given, and HOME define go to HOME
*/
/*********************************************************************/
int	builtin_cd(t_token *token, t_data *data)
{
	char	*path;

	if (!token || !token->next || !token->next->str)
	{
		path = getenv("HOME");
		if (!path)
			return (perror("cd : HOME not set\n"), 1);
	}
	else
	{
		if (token->next->to_expand == true)
			path = expand_quotes(token->next->str, data, token);
		else
			path = token->next->str;
	}
	if (!is_valid_dir(path))
	{
		data->last_exit = 1;
		return (data->last_exit);
	}
	if (go_to(data, path))
	{
		update_pwd(data);
		data->last_exit = 0;
	}
	// printf("exit code = %d\n", data->last_exit);
	return (data->last_exit);
}
