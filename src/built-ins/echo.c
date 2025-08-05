/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moaljazz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:22:51 by moaljazz          #+#    #+#             */
/*   Updated: 2025/06/18 19:22:54 by moaljazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int echo(char *cmd)
{
    char **split_;
    int i;
    bool is_n = false;

    split_ = ft_split(cmd, ' ');
    if (!split_)
        return 1;
    i = 0;
    if (ft_strcmp(split_[i], "-n") == 0)
    {
        is_n = true;
        i++;
    }
    while (split_[i]) 
    {
        if (is_n)
            printf("%s", split_[i]);
        else
            printf("%s\n", split_[i]);
        i++;
    }
    free(split_);
    return 0;
}
