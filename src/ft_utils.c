/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramroma <ramroma@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 22:48:22 by moaljazz          #+#    #+#             */
/*   Updated: 2025/07/18 11:29:05 by ramroma          ###   ########.fr       */
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

	full_path = malloc(ft_strlen(dir) + ft_strlen(cmd) + 2);
	if (!full_path)
		return (NULL);
	ft_strcpy(full_path, dir);
	ft_strcat(full_path, "/");
	ft_strcat(full_path, cmd);
	return (full_path);
}

void	fork1(int infile, char **ag, char **env, int *pd)
{
	if (file_errhandle(infile) == 0)
	{
		first_fork_operate(infile, ag, env, pd);
	}
	else
	{
		close(pd[0]);
		close(pd[1]);
		exit(1);
	}
	return ;
}

void	fork2(int outfile, char **ag, char **env, int *pd)
{
	if (file_errhandle(outfile) == 0)
		second_fork_operate(outfile, ag, env, pd);
	else
	{
		close(pd[0]);
		close(pd[1]);
		exit(1);
	}
	return ;
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

// char	*bash_cmd(char *agv)
// {
// 	char	*new_cmd;

// 	if (!agv || ft_strncmp(agv, "./", 2) != 0)
// 	{
// 		return (ft_strdup(agv));
// 	}
// 	if (ft_strncmp(agv, "./ ", 3) == 0)
// 	{
// 		return (ft_strdup(agv));
// 	}
// 	new_cmd = ft_strjoin("bash ", agv + 2);
// 	return (new_cmd);
// }
