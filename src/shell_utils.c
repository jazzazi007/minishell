/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 11:37:05 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/08/03 17:30:22 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int ft_add_cmd(t_cmd **r_root)
{
	t_cmd   *new;
	t_cmd   *curr;

	new = ft_init_cmd();
	if (!new)
		return (-1);
	curr = *r_root;
	if (*r_root == NULL)
	{
		*r_root = new;
		return (0);
	}
	while(curr)
		curr = curr->next;
	curr->next = new;
	return (0);
}

t_cmd   *ft_init_cmd()
{
    t_cmd   *new;

    new = malloc(sizeof(t_cmd));
    if (!new)
        return (NULL);
    ft_bzero(new, sizeof(new));
	new->here_fd = -1;
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
