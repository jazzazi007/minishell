/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 22:48:22 by moaljazz          #+#    #+#             */
/*   Updated: 2025/08/18 16:40:03 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	file_close(int file_closing)
{
	if (file_closing >= 0)
		close(file_closing);
	return ;
}

char	*get_cmd_assist(char *cmd, char *dir)
{
	char	*full_path;

	// if (!dir)
	// 	return(NULL);
	// printf("cmd:%s\ndir:%s\n",cmd,dir);
	full_path = malloc(ft_strlen(dir) + ft_strlen(cmd) + 2);
	if (!full_path)
		return (NULL);
	ft_strcpy(full_path, dir);
	ft_strcat(full_path, "/");
	ft_strcat(full_path, cmd);
	return (full_path);
}

char *strip_quotes(const char *str)
{
    int len ;

	len = ft_strlen(str);
    if (len >= 2)
    {
        if ((str[0] == '"' && str[len - 1] == '"') || (str[0] == '\'' && str[len - 1] == '\''))
            return ft_substr(str, 1, len - 2);
    }
    return ft_strdup(str);
}
