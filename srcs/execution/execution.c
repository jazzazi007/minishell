/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felayan <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:23:45 by moaljazz          #+#    #+#             */
/*   Updated: 2025/09/22 22:47:26 by felayan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_pipes(t_shell *sh)
{
	t_cmd	*tmp;

	tmp = sh -> cmds;
	while (tmp)
	{
		tmp -> fds[0] = -1;
		tmp -> fds[1] = -1;
		if (tmp -> next)
		{
			if (pipe(tmp -> fds) < 0)
			{
				perror("minishell: pipe");
				sh -> exit = errno;
				close_fds();
				return (sh -> exit);
			}
		}
		tmp = tmp -> next;
	}
	return (SUCCESS);
}

int	built_ins(t_cmd *cmd, t_shell *shell)
{
	if (!ft_strcmp(cmd->args[0], "cd"))
		return (cd(cmd -> args, shell));
	if (!ft_strcmp(cmd->args[0], "exit"))
		return (exit_command(cmd, shell), 0);
	else if (!ft_strcmp(cmd->args[0], "echo"))
		return (echo(cmd->args));
	else if (!ft_strcmp(cmd->args[0], "export"))
		return (export_cmd(cmd->args, shell));
	else if (!ft_strcmp(cmd->args[0], "unset"))
		return (unset_cmd(cmd->args, shell));
	else if (!ft_strcmp(cmd->args[0], "env"))
		return (env(shell, cmd));
	else if (!ft_strcmp(cmd->args[0], "pwd"))
		return (pwd(shell));
	return (1);
}

static int	parent_builtin_exec(t_shell *sh, t_cmd *cmd)
{
	int	stdfd[2];

	stdfd[0] = dup(STDIN_FILENO);
	stdfd[1] = dup(STDOUT_FILENO);
	if (stdfd[0] < 0 || stdfd[1] < 0)
	{
		perror("minishell: dup");
		sh -> exit = errno;
		ft_close_fdpair(stdfd);
		return (sh -> exit);
	}
	if (open_dup_fds(STDIN_FILENO, STDOUT_FILENO, sh, cmd) == SUCCESS)
		built_ins(cmd, sh);
	if (dup2(stdfd[0], STDIN_FILENO) < 0 || dup2(stdfd[1], STDOUT_FILENO) < 0)
	{
		perror("minishell: dup");
		sh -> exit = errno;
		ft_close_fdpair(stdfd);
		return (sh -> exit);
	}
	ft_close_fdpair(stdfd);
	return (SUCCESS);
}

void	execution(t_shell *sh)
{
	t_cmd	*cmd;
	t_cmd	*prev;

	prev = NULL;
	cmd = sh -> cmds;
	if (init_pipes(sh))
		return ;
	if (sh -> cmd_count == 1 && is_parent_builtin(cmd -> args[0]))
	{
		if (parent_builtin_exec(sh, cmd))
			return ;
	}
	else
	{
		while (cmd)
		{
			sh -> exit = 0;
			if (child_fork(cmd, prev, sh))
				return ;
			prev = cmd;
			cmd = cmd -> next;
		}
	}
	cleanup_resources(sh);
}
