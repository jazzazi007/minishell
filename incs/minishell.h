/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felayan <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 21:32:57 by felayan           #+#    #+#             */
/*   Updated: 2025/09/24 17:53:04 by felayan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define MALLOC_FAILURE 42
# define SYNTAX_ERR -1
# define SUCCESS 0

# define BBLU "\033[1;34m"
# define RST "\033[0m"

# define _DEFAULT_SOURCE

# include <readline/readline.h>
# include <readline/history.h>
# include <linux/limits.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <stdio.h>
# include "libft.h"

typedef struct s_tokens			t_tokens;
typedef struct s_redir			t_redir;
typedef struct s_shell			t_shell;
typedef struct s_cmd			t_cmd;
typedef enum e_rdr				t_rdr;
typedef enum e_tk				t_tk;
extern volatile sig_atomic_t	g_exit_status;

enum e_tk
{
	T_WORD,
	T_DOUBLE,
	T_SINGLE,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_HEREDOC,
	T_APPEND,
	T_FILE
};

enum e_rdr
{
	IN,
	OUT,
	APPEND,
	HEREDOC
};

struct s_tokens
{
	bool		is_expandable;
	char		*value;
	t_tk		type;
	t_tokens	*next;
};

struct s_redir
{
	t_rdr	type;
	char	*filename;
	int		doc_fd;
};

struct s_cmd
{
	int		redir_count;
	int		word_count;
	char	*cmd_path;
	int		fds[2];
	char	**args;
	t_redir	*redir;
	t_cmd	*next;
	pid_t	pid;
};

struct s_shell
{
	char		**envp;
	t_cmd		*cmds;
	int			cmd_count;
	int			exit;
	pid_t		last_cmd_pid;
	t_tokens	*tokens;
};

void	cleanup_resources(t_shell *sh);
void	clean_shell(t_shell *shell, int status);
void	clean_tokens(t_tokens *head);
void	clean_cmds(t_cmd *cmds);
void	clean_strs(char **strs);

char	*append_str(char **env, char *s1, const char *s2, bool is_special);
char	*append_char(char *s, char c);
int		add_redir_cmd(t_cmd *cmd, t_tokens **tokens, int *rdr_i, t_shell *sh);
int		crt_var(char **env, char **expanded, char *key, bool is_special);
int		add_word_cmd(t_cmd *cmd, const char *token, int *wrd_i);
int		syntax_check(t_tokens *tokens, bool quotes_err);
int		tokenizer(const char *input, t_shell *shell);
int		parsing(t_shell *shell, const char *input);
void	add_token(t_shell *sh, t_tk t_type, char *token, bool exp);
void	add_single_quotes(t_shell *sh, const char *input, int *i);
void	add_double_quotes(t_shell *sh, const char *input, int *i);
void	add_operator(t_shell *sh, const char *input, int *i);
void	add_word(t_shell *sh, const char *input, int *i);
void	expander(t_shell *sh);
bool	is_closed_quotes(const char *input, int loc);
t_cmd	*init_cmd(t_shell *sh, t_tokens *tokens);
t_cmd	*get_last_cmd(t_cmd *cmd);

int		count_env_entries(char **envp);
char	*get_env_value(const char *key, char **envp);
void	init_env(t_shell *sh, char **env);
void	ft_close_fdpair(int fd[2]);
void	close_fds(void);
bool	is_redir(const char *token);
bool	is_empty(const char *line);
bool	is_pipe(const char *token);
bool	is_word(const char *token);
bool	is_var(char var);
t_tk	get_opertype(const char *s);
int		skip_whitesp(const char *s);
int		is_oper(char c);
char	*ret_fail(char **paths, char *arg, t_shell *sh);

int		open_dup_fds(int fd_in, int fd_out, t_shell *sh, t_cmd *cmd);
void	cmd_exec(t_cmd *agv, t_shell *shell);
char	*resolve_path(char *cmd0, t_shell *shell);
void	execution(t_shell *sh);
bool	is_parent_builtin(const char *cmd);
int		child_fork(t_cmd *cmd, t_cmd *prev, t_shell *sh);
void	child_exec(t_shell *sh, t_cmd *curr, t_cmd *prev);

char	*append_expanded_part(char *res, char *input, int *i, t_shell *sh);
char	*ft_var_expand(const char *str, int *i, t_shell *sh);
char	*ft_strappend(char *dst, const char *src);
int		ft_open_heredoc(t_tokens *delim, t_shell *shell);

void	exit_command(t_cmd *cmd, t_shell *shell);
int		export_cmd(char **args, t_shell *shell);
int		unset_cmd(char **args, t_shell *shell);
int		built_ins(t_cmd *cmd, t_shell *shell);
int		cd(char **av, t_shell *sh);
int		env(t_shell *shell, t_cmd *cmd);
int		echo(char **cmd);
int		pwd(t_shell *sh);
void	set_var(t_shell *sh, const char *var);
bool	is_valid_key(const char *name);

void	setup_signal_handlers(void);
void	sigint_handler(int signum);
void	handle_herdoc_c(int sig);
void	signal_exc_parent(void);
void	handle_quit(int sig);
void	signal_excuter(void);
void	handle_herdoc(void);
void	handle_c(int sig);

#endif