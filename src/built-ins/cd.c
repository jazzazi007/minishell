/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moaljazz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:22:39 by moaljazz          #+#    #+#             */
/*   Updated: 2025/06/18 19:22:43 by moaljazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int cd(char **av, char **envp)
{
    char path[PATH_MAX];
    char *home;

    (void)envp;

    // If no argument, go to HOME
    if (!av[1])
    {
        home = getenv("HOME");
        if (!home)
        {
            fprintf(stderr, "cd: HOME not set\n");
            return 1;
        }
        if (chdir(home) != 0)
        {
            perror("cd");
            return 1;
        }
    }
    else
    {
        if (chdir(av[1]) != 0)
        {
            perror("cd");
            return 1;
        }
    }

    // Optionally update PWD environment variable here

    if (getcwd(path, sizeof(path)) == NULL)
        perror("getcwd");

    return 0;
}
