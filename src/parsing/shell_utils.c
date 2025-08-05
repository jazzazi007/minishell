/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 11:37:05 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/08/04 14:34:15 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_cmd *ft_add_cmd(t_cmd **r_root)
{
	t_cmd *new;
	t_cmd *curr;

	new = ft_init_cmd();
	if (!new)
		return (NULL);
	if (*r_root == NULL)
	{
		*r_root = new;
		return (new);
	}
	curr = *r_root;
	while (curr->next)
		curr = curr->next;
	curr->next = new;
	return (new);
}


t_redir *ft_add_redir(t_redir **r_root)
{
	t_redir   *new;
	t_redir   *curr;

	new = ft_init_redir();
	if (!new)
		return (NULL);
	curr = *r_root;
	if (*r_root == NULL)
	{
		*r_root = new;
		return (new);
	}
	while(curr)
		curr = curr->next;
	curr->next = new;
	return (new);
}

t_redir	*ft_init_redir()
{
	t_redir *new;

	new = malloc(sizeof(t_redir));
	if (!new)
		return (NULL);
	ft_memset(new, 0, sizeof(new));
	new->here_fd = -1;
	new->next = NULL;
	return (new);
}

t_cmd   *ft_init_cmd()
{
    t_cmd   *new;

    new = malloc(sizeof(t_cmd));
    if (!new)
        return (NULL);
    ft_bzero(new, sizeof(new));
	// new->redir = ft_init_redir();
	// if (!new->redir)
	// {
	// 	free(new);
	// 	return (NULL);
	// }
	new->redir = NULL;
	new->next = NULL;
    return (new);
}

t_minishell *ft_init_shell(char **envp)
{
    t_minishell *new;

	new = malloc(sizeof(t_minishell));
	if (!new)
        return (NULL);
    new->input = NULL;
    new->envp = envp;
    new->cmds = NULL;
    new->pipes[0] = NULL;
    new->pipes[1] = NULL;
    new->token_list = NULL;
    new->token_count = -1;
    new->last_exit = 0;
	return (new);
}

