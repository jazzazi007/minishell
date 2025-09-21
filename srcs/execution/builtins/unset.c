/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:41:48 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/08/19 12:41:50 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int find_env(char **envp, const char *name)
{
    int i = 0;
    size_t len = strlen(name);
    while (envp[i])
    {
        if (strncmp(envp[i], name, len) == 0) //removed 2nd condition to fix unset for no value keys
            return i;
        i++;
    }
    return -1;
}

static void remove_env(char ***envp, int index)
{
    int count = 0;
    while ((*envp)[count])
        count++;
    free((*envp)[index]);
    for (int i = index; i < count - 1; i++)
        (*envp)[i] = (*envp)[i + 1];
    (*envp)[count - 1] = NULL;
}

int unset_cmd(char **args, t_shell *shell)
{
    int i = 1;
    int ret = 0;
    if (!args[1])
        return (0);
    while (args[i])
    {
        int idx = find_env(shell->envp, args[i]);
        if (idx != -1)
            remove_env(&shell->envp, idx);
        i++;
    }
    return ret;
}