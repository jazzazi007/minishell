/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   return.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moaljazz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 21:50:46 by moaljazz          #+#    #+#             */
/*   Updated: 2025/03/23 21:50:49 by moaljazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

char	*null_ret(char *path)
{
	free(path);
	return (NULL);
}

char	*path_ret(char *path, char *cpy)
{
	free(cpy);
	return (ft_strdup(path));
}

int	handle_ret_num(char *path, char **cmd, int err_num)
{
	free(path);
	free_split(cmd);
	return (err_num);
}

int	handle_ret(char *path, char **cmd, int err_num)
{
	write(2, cmd[0], ft_strlen(cmd[0]));
	write(2, ": command not found\n", 20);
	return (handle_ret_num(path, cmd, err_num));
}

int	exceve_ret(char *path, char **cmd, int err_num)
{
	errno = ENOKEY;
	perror("Error");
	return (handle_ret_num(path, cmd, err_num));
}
