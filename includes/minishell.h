/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcali <tcali@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 18:30:16 by tcali             #+#    #+#             */
/*   Updated: 2025/09/22 10:45:37 by tcali            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "../libft/includes/libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <stdbool.h>
# include <limits.h>
# include "colors.h"

extern int g_exit_status;
/* ************************************************************************** */
/* ********************************Structures******************************** */
typedef enum e_tkn_code
{
	CMD,
	ARG,
	IN,
	OUT,
	APPEND,
	HEREDOC,
	PIPE,
	AND,
	OR,
	SEP,
}		t_tkn_code;

typedef enum e_print_code
{
	OFF,
	PRE,
	POST,
}			t_print_code;

typedef struct s_input
{
	char	*line;
	char	*new_line;
	char	*remaining;
	char	*quoted;
	char	*token;
	char	quote;
	bool	*quotes;
}			t_input;

typedef struct s_token
{
	char			*str;
	int				pos;
	t_tkn_code		type;
	int				input;
	int				output;
	bool			to_expand;
	bool			join_next;
	struct s_token	*prev;
	struct s_token	*next;
}					t_token;

typedef struct s_env
{
	char			*variable;
	char			*value;
	bool			exported;
	struct s_env	*prev;
	struct s_env	*next;
}					t_env;

typedef struct s_data
{
	int		**pipe_fd;
	pid_t	pid;
	pid_t	*pids;
	char	*line;
	char	**tokens;
	char	**cmds;
	bool	array_alloc;
	t_token	*token;
	t_env	*exported_env;
	char	**envp;
	bool	env_set;
	bool	expand;
	int		nb_tokens;
	int		nb_pipes;
	int		total_pipes;
	int		nb_cmds;
	int		exit_code;
	int		last_exit;
	char	*heredoc;
	char	*delimiter;
}		t_data;

/* ************************************************************************** */
/* ********************************Functions********************************* */

//* ****************************** BUILTINS

//builtins.c (builtins/)
int		is_builtin(char *cmd);
int		builtin_echo(t_data *data, t_token *token);
int		builtin_exit(t_data *data);
int		builtin_pwd(t_token *token);

// builtin_cd.c
char	*update_env_pwd(t_data *data, char *cwd);
int		builtin_cd(t_token *token, t_data *data);

//env.c (builtins/)
void	export_to_env(t_data *data);
t_env	*find_env(t_data *data, char *var);
int		builtin_env(t_data *data);

// export.c (builtins/)
t_env	*parse_export(t_token *token);
t_env	*new_exported(char *var);
void	export_add_back(t_env **env, t_env *new);
int		builtin_export(t_data *data, t_token *token);
void	add_or_update_export(t_data *data, t_env *new, int len_variable);

//unset (builtins/)
t_env	*find_var(char *var, t_data *data);
int		delete_from_export(char *var, t_data *data);
void	delete_var(char *found, t_data *data);
int		builtin_unset(t_data *data, t_token *token);

//update_env.c (builtins/)
void	env_to_export(char **envp, t_env **exported);
void	add_to_env(t_data *data, char *var);
void	sort_export(t_env *exported);
void	update_export(t_env *env, t_env *new, char *value, bool append);
bool	is_append(t_env *new, int len_variable);

//* ****************************** CMD_MANAGEMENT

//exec.c (cmd_management/)
int		split_cmd(char *command, char **env, char ***args, char **path);
int		exec_builtin(t_token *token, t_data *data);
void	exec_failed(char **path, char ***args, t_data *data);
void	execute_command(char *command, char **env,
			t_token *token, t_data *data);

//fork.c (cmd_management/)
void	child(t_data *data, int i, t_token *token);
void	parent(t_data *data);
void	create_child(t_data *data, t_token *token);
void	fork_process(t_data *data);

//path.c (cmd_management/)
char	*find_cmd_path(char *cmd, char **path);
char	*get_cmd_path(char *cmd, char **env);
int		is_valid_dir(char *path);
int		check_cdpath(t_data *data, char *path);
int		go_to(t_data *data, char *path);

//* ****************************** DEBUG

//debug.c (debug/)
void	print_current_token(t_token *current);
void	print_tokens(t_data *data);
void	reset_colors(void);
void	print_array(char **array, bool debug);
void	print_current_export(t_env *current);

//* ****************************** MAIN

//init.c (main/)
int		count_pipes(t_data *data, t_token *current, bool total);
void	init_pipes(t_data *data, int n);
void	init_pids(t_data *data);
void	init_env(t_data *data, char **env);
int		init_data(t_data *data);

//signals.c (main/)
void	handle_sigint(int sig);
void	setup_heredoc_signals(void);
void	check_sigint_sigquit(int status, t_data *data);

//token_management.c (main/)
t_token	*new_token(char *str);
void	token_add_back(t_token **lst, t_token *new);
int		create_add_token(t_data *data);
t_token	*add_arg(t_token *current, t_data *data);

//* ****************************** PARSING

//expand.c
char	*expand_quotes(char *var, t_data *data, t_token *token);
char	*var_to_value(char *exp, t_token *current);

// parse_pipe.c
int		check_empty_pipes(char *line);

//parse.c (parsing/)
void	token_to_array(t_token *start, t_data *data);
// char	**parse_line(const char *line);
int		parse_line(t_data *data, t_input *input);

// parsing_errors.c
int		is_bad_redirect(char c);
int		syntax_and(char *str);
int		syntax_error(char *str);

//parsing_quotes.c
char	*copy_token(char *line, int start, int i, t_data *data);
int		quote_is_closed(char *line, char quote);

// redirect.c
int		triple_check(char *line, char c);
int		redirect_inout(char *line);
int		append_inout(char *line);

//* ****************************** REDIRECTION

// heredoc.c (redirection/)
int		heredoc_present(t_token *token);
void	free_heredoc(t_data *data);
int		write_heredoc(char *delimiter, int tmp_fd);
int		handle_heredoc(t_data *data, char *delimiter);
int		check_heredoc(t_token *token, t_token *cmd, t_data *data);

// pipes.c (redirection/)
t_token	*create_pipeline(t_data *data, t_token *start);

//redirection.c (redirection/)
void	check_redirection(t_token *token, t_data *data);
int		redirect_stream(t_token *token, t_token *cmd, t_data *data);
// void	reset_stream(t_data *data);

//* ****************************** UTILS

//free.c (utils/)
void	free_minishell(t_data *data, bool end);
void	free_array(char ***array);
void	free_list(t_token **lst);
void	free_pipes(int **pipes, int n, t_data *data);
void	free_pids(t_data *data);

//free2.c (utils/)
void	free_exported(t_env *exported);
void	free_env(t_data *data);

//utils.c (utils/)
void	close_all_pipes(int **pipes, int n, t_data *data);
int		check_type(t_token *token, t_data *data);
int		check_and_or(t_token *token, t_data *data);
void	wait_all(t_data *data, int *last_status);

//utils2.c (utils/)
int		no_fork_needed(char *cmd);
t_token	*skip_to_next(t_token *token, t_tkn_code type);
void	update_position(t_token *token);
int		count_cmds(t_token *token);
t_token	*find_prev_cmd(t_token *token);

// utils3.c (utils/)
char	**array_dup(char **array);
void	print_export(t_env *exported);
int		is_valid_identifier(char *str);
int		is_redirection(t_tkn_code type);
void	print_error(char *error, t_print_code code, t_token *token);
void	safe_print(char *str, bool free, bool newline);

// utils4.c
int		count_tokens(t_token *token);

/* ************************************************************************** */
//lbft
char	*ft_str_threejoin(char const *s1, char const *s2, char const *s3);

#endif
/*
Couleur	Code
Noir	\033[0;30m
Rouge	\033[0;31m
Vert	\033[0;32m
Jaune	\033[0;33m
Bleu	\033[0;34m
Magenta	\033[0;35m
Cyan	\033[0;36m
Gris clair	\033[0;37m
Reset (par défaut)	\033[0m
*/