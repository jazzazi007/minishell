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
//# include <string.h>
# include <unistd.h>
//# include <dirent.h>
# include <fcntl.h>
# include <errno.h>
# include <stdio.h>
# include "libft.h"

typedef struct s_tokenizer		t_tokenizer;
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

struct s_tokenizer
{
	bool		is_expandable;
	char		*value;
	t_tk		type;
	t_tokenizer	*next;
};

struct s_redir
{
	t_rdr	red_type;
	char	*filename;
	int		here_fd;
};

struct s_cmd
{
	int					redir_count;
	int					word_count;
	char				*cmd_path;
	char				**args;
	t_redir				*redir;
	struct s_cmd		*next;
};

struct s_shell
{
	char				**envp;
	t_cmd				*cmds;
	int					cmd_count;
	int					exit_status;
	int					**fds;
	t_tokenizer			*tokens;
};

void	cleanup_resources(int **fds, pid_t *pids, int count, t_shell *shell);
void 	clean_shell(t_shell *shell, int status);
void	clean_tokens(t_tokenizer *head);
void	clean_cmds(t_cmd *cmds);
void	clean_env(char **envp);

char	*append_str(char **env, char *s1, const char *s2, bool is_special);
char	*append_char(char *s, char c);
int		crt_var(char **env, char **expanded, char *key, bool is_special);
int		expand_var(t_shell *dt, const char *token, char **expanded);
int		syntax_check(t_tokenizer *tokens, bool quotes_err);
int		tokens_to_cmd(t_shell *dt, t_tokenizer *tokens);
int		tokenizer(const char *input, t_shell *shell);
int		parsing(t_shell *shell, const char *input);
void	add_token(t_shell *dt, t_tk t_type, char *token, bool exp);
void	add_single_quotes(t_shell *dt, const char *input, int *i);
void	add_double_quotes(t_shell *dt, const char *input, int *i);
void	add_operator(t_shell *dt, const char *input, int *i);
void	add_word(t_shell *dt, const char *input, int *i);
void	expander(t_shell *dt);
bool	is_closed_quotes(const char *input, int loc);


char	*get_env_value(const char *key, char **envp);
void	init_env(t_shell *dt, char **env);
void	ft_close_fdpair(int fd[2]);
void	close_fds(void);
bool	is_redir(const char *token);
bool	is_empty(const char *line);
bool	is_pipe(const char *token);
bool	is_word(const char *token);
bool	is_var(char var);
t_tk	get_opertype(const char *s);
int		skip_whitesp(const char *s);
int		count_pipes(t_cmd *ag);
int		is_oper(char c);

int		close_pipes(int outfile, int *pd, pid_t id, pid_t id2);
int		fork_operate(int fd_in, int fd_out, t_cmd *cmd);
int		cmd_exec(t_cmd *agv, t_shell *shell);
int		**init_pipes(int pipe_count);
int		check_fork(pid_t id);
pid_t	*init_child_pids(int pipe_count, int **pipe_fds);
char	*resolve_cmd_path(char *cmd0, t_shell *shell);
char	*get_cmd_assist(char *cmd, char *dir);
char	*get_cmd_path(char *cmd, char **env);
char	*strip_quotes(const char *str);
void	check_pipes_forks(t_shell *sh);
void	file_close(int file_closing);
void	free_split(char **cmd);

char	*path_ret(char *path, char *cpy);
char	*null_ret(char *path);
int		handle_ret(char *path, char **cmd, int err_num);
int		handle_ret_num(char *path, char **cmd, int err_num);
int		exceve_ret(char *path, char **cmd, int err_num);


char	*append_expanded_part(char *res, char *input, int *i, t_shell *sh);
char	*ft_var_expand(const char *str, int *i, t_shell *sh);
char	*ft_strappend(char *dst, const char *src);
int		ft_open_heredoc(t_tokenizer *delim, t_shell *shell);
void	ft_strstrip(char **str_r);

void	exit_command(t_cmd *cmd, t_shell *shell);
int		export_cmd(char **args, t_shell *shell);
int		unset_cmd(char **args, t_shell *shell);
int		built_ins(t_cmd *agv, t_shell *shell);
int		cd(char **av, char **envp);
int		env(t_shell *shell);
int		echo(char **cmd);
int		pwd(void);

void	setup_signal_handlers(void);
void	sigint_handler(int signum);
void	handle_herdoc_c(int sig);
void	signal_exc_parent(void);
void	handle_quit(int sig);
void	signal_excuter(void);
void	handle_herdoc(void);
void	handle_c(int sig);

#endif