/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:28:03 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/07/17 19:06:52 by ralbliwi         ###   ########.fr       */
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
}	t_tokentype;

typedef struct s_tokenizer
{
    char                *value;
    t_tokentype         type;
    struct s_tokenizer  *next;
}   t_tokenizer;

t_tokenizer *tokenize_input(const char *input);
void free_tokens(t_tokenizer *head);
t_tokenizer *new_token(const char *val, t_tokentype type);
void add_token(t_tokenizer **head, t_tokenizer *new);
///

typedef struct minishell
{
	char			*input;
	char			**envp;
	char			**cmds;
	int				exit_status;
	int				*pipes[2];
	t_tokenizer		*token_list;
	int				token_count;
}	t_minishell;
//execute 

int		cmd_exec(char *agv, char **envp);
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
char	*bash_cmd(char *agv);
void	file_close(int file_closing);
char	*path_ret(char *path, char *cpy);
char	*null_ret(char *path);
int		handle_ret(char *path, char **cmd, int err_num);
int		handle_ret_num(char *path, char **cmd, int err_num);
int		exceve_ret(char *path, char **cmd, int err_num);
void check_pipes_forks(char *ag, char **env);
bool is_valid_pipe_syntax(char *ag);
void fork_operate(int fd_in, char *cmd, char **env, int *pipe_fd);
int **init_pipes(int pipe_count);
pid_t *init_child_pids(int pipe_count, int **pipe_fds);
void cleanup_resources(int **pipe_fds, pid_t *child_pids, int pipe_count);
int count_pipes(char *ag);
char *get_command(char *input, int position);
/////////

//signal
extern int g_exit_status;

void sigint_handler(int signum);
void setup_signal_handlers(void);
///

char **build_argv(t_tokenizer *tokens);
#endif