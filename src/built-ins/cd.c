/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:22:39 by moaljazz          #+#    #+#             */
/*   Updated: 2025/08/19 16:22:32 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int cd(char **av, char **envp)
{
    char path[PATH_MAX];
    char *home;
    int status;

    (void)envp;

    status = 0;
    if (!av[1])
    {
        home = getenv("HOME");
        if (!home)
        {
            printf("cd: HOME not set\n");
            status = 1;
        }
        if (chdir(home) != 0)
        {
            perror("cd");
            status =  1;
        }
    }
    else if(av[2])
    {
        printf("minishell: cd: too many arguments\n");
        status = 1;
    }
    else
        if (chdir(av[1]) != 0)
        {
            perror("cd");
            status = 1;
        }
    
    if (getcwd(path, sizeof(path)) == NULL)
        perror("getcwd");
    return(status);
}
