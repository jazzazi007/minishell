/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:28:03 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/08/08 10:41:33 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdlib.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
# include <errno.h>
#include "../libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <linux/limits.h>
#include <dirent.h>

//tokenizer
typedef enum e_tokentype
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_HEREDOC,
	T_APPEND,
	T_FILE,
}	t_tokentype;

typedef struct s_tokenizer
{
    char                *value;
    t_tokentype         type;
    struct s_tokenizer  *next;
}   t_tokenizer;

typedef struct s_redir
{
	int 			red_type; //0 ouput, 1 append, 2 input, 3 heredoc
	char			*filename; //incase of heredoc, filename is the same as delim			
	int				here_fd;
	struct s_redir 	*next;
}	t_redir;

typedef struct s_cmd
{
	char			*cmd_path;
	char			**args;
	t_redir			*redir;
	struct s_cmd 	*next;
}	t_cmd;

typedef struct minishell
{
	char			**envp;
	t_cmd			*cmds;
	int				exit_status;
}	t_minishell;

t_tokenizer		*tokenize_input(const char *input, t_minishell *shell);
void 			free_tokens(t_tokenizer *head);
t_tokenizer 	*new_token(const char *val, t_tokentype type);
void 			add_token(t_tokenizer **head, t_tokenizer *new);
int 			is_metachar(char c);
t_tokentype 	get_metatype(const char *s);
int			is_syntax_error(t_tokenizer **r_tokens, t_minishell *shell);

//execute 

int		cmd_exec(char *agv, t_minishell *shell);
char	*get_cmd_path(char *cmd, char **env);
char	*ft_getenv(char **env);
void	pipex(int infile, int outfile, char **ag, char **env);
void	free_split(char **cmd);
int		close_pipes(int outfile, int *pd, pid_t id, pid_t id2);
int		check_fork(pid_t id);
void	first_fork_operate(int infile, char **ag, char **env, int *pd);
void	second_fork_operate(int outfile, char **ag, char **env, int *pd);
int		file_errhandle(int file);
char	*get_cmd_assist(char *cmd, char *dir);
void	fork1(int infile, char **ag, char **env, int *pd);
void	fork2(int outfile, char **ag, char **env, int *pd);
char *strip_quotes(const char *str);
void	file_close(int file_closing);
char	*path_ret(char *path, char *cpy);
char	*null_ret(char *path);
int		handle_ret(char *path, char **cmd, int err_num);
int		handle_ret_num(char *path, char **cmd, int err_num);
int		exceve_ret(char *path, char **cmd, int err_num);
void 	check_pipes_forks(char *ag, t_minishell	*sh);
bool 	is_valid_pipe_syntax(char *ag);
void 	fork_operate(int fd_in, char *cmd, char **env, int *pipe_fd);
int 	**init_pipes(int pipe_count);
pid_t 	*init_child_pids(int pipe_count, int **pipe_fds);
void ft_close_fdpair(int fd[2]);
void 	cleanup_resources(int **pipe_fds, pid_t *child_pids, int pipe_count);
int 	count_pipes(char *ag);
char 	*get_command(char *input, int position);
/////////

//signal
extern int g_exit_status;

void 	sigint_handler(int signum);
void 	setup_signal_handlers(void);
///

//expand
char	*append_expanded_part(char *res, char *input, int *i,
	t_minishell *sh, int in_single);
char	*expand_token_value(char *input, t_minishell *sh);
char	*expand_var(const char *str, int *i, t_minishell *sh);
char	*ft_strappend(char *dst, const char *src);
char	*resolve_cmd_path(char *cmd0, t_minishell *shell);
int	expand_tokens(t_tokenizer **r_head, t_minishell *sh);
char	*get_env_value(const char *key, char **envp);
int ft_strstrip(char **str_r);
int ft_strstrip(char **str_r);


t_cmd 	*build_cmd(t_tokenizer **tokens, t_minishell *shell);
void 	ft_print_cmd(t_cmd **r_cmds);
void ft_print_tokens(t_tokenizer **r_tokens);

t_cmd   		*ft_init_cmd();
t_minishell 	*ft_init_shell(char **envp);
t_cmd 			*ft_add_cmd(t_cmd **r_rootcmd);
t_redir			*ft_init_redir();
t_redir 			*ft_add_redir(t_redir **r_root);

void 			ft_free_redir(t_redir *redir);
void 			ft_free_args(char **args);
void 			ft_free_cmd(t_cmd *cmd);
void 			ft_free_cmds(t_cmd *cmds);
void 			ft_free_shell(t_minishell *shell);

int 			ft_indicate_error(const char *msg, int exit_code, t_minishell *shell);
int 			ft_parse_cmd(t_minishell *shell, const char *input);

int 			ft_is_redir(char *s);
int 			ft_fill_redir(t_redir **r_redir, t_tokenizer *curr, t_minishell *shell);
int ft_open_heredoc(char *delim, t_minishell *shell);

#endif