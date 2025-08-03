/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 11:37:05 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/08/03 12:06:39 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
