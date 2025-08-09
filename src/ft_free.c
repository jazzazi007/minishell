/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 11:55:39 by codespace         #+#    #+#             */
/*   Updated: 2025/08/09 10:39:34 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void ft_free_redir(t_redir *redir)
{
	t_redir *tmp;

	if (!redir)
		return ;
	while (redir)
	{
		tmp = redir->next;
        free(redir->filename);
		free(redir);
		redir = tmp;
	}
}

void ft_free_args(char **args)
{
	int i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void ft_free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	ft_free_redir(cmd->redir);
	ft_free_args(cmd->args);
	// free(cmd->cmd_path);
	free(cmd);
}

void ft_free_cmds(t_cmd *cmds)
{
	t_cmd *tmp;

	if (!cmds)
		return ;
	while (cmds)
	{
		tmp = cmds->next;
		ft_free_cmd(cmds);
		cmds = tmp;
	}
}

void ft_free_shell(t_minishell *shell)
{
	if (!shell)
		return ;
	if (shell->cmds)
	{
		ft_free_cmds(shell->cmds);
		shell->cmds = NULL; // Prevent double free
	}
	if (shell->envp)
	{
		ft_free_envp(shell->envp);
		shell->envp = NULL; // Prevent double free
	}
	free(shell);
}
