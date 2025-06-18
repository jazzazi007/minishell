/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 17:59:42 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/02/12 18:54:08 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_free(char *buff, char *savedbuff)
{
	free(savedbuff);
	free(buff);
	return (NULL);
}

static char	*read_buff(char *buff, char *savedbuff, int fd)
{
	ssize_t	bytesread;
	char	*temp;

	while (!ft_strchr_gnl(savedbuff, '\n'))
	{
		bytesread = read(fd, buff, BUFFER_SIZE);
		if (bytesread == -1)
		{
			ft_free(buff, savedbuff);
			return (NULL);
		}
		else if (bytesread == 0)
			break ;
		buff[bytesread] = '\0';
		temp = ft_strjoin_gnl(savedbuff, buff);
		if (!temp)
		{
			ft_free(temp, buff);
			return (NULL);
		}
		free(savedbuff);
		savedbuff = temp;
	}
	free(buff);
	return (savedbuff);
}

static char	*next_line(char *savedbuff)
{
	int		i;
	int		j;
	char	*line;

	i = 0;
	j = 0;
	if (!savedbuff || !savedbuff[i])
		return (NULL);
	while (savedbuff[i] && savedbuff[i] != '\n')
		i++;
	if (savedbuff[i] == '\n')
		line = malloc(sizeof(char) * (i + 2));
	if (savedbuff[i] == '\0')
		line = malloc(sizeof(char) * (i + 1));
	if (!line)
		return (NULL);
	i = 0;
	while (savedbuff[i] && savedbuff[i] != '\n')
		line[j++] = savedbuff[i++];
	if (savedbuff[i] == '\n')
		line[j++] = savedbuff[i];
	line[j] = '\0';
	return (line);
}

static char	*save(char *savedbuff)
{
	char	*new_save;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (savedbuff[i] && savedbuff[i] != '\n')
		i++;
	if (!savedbuff[i])
	{
		free(savedbuff);
		return (NULL);
	}
	i++;
	new_save = malloc(sizeof(char) * (ft_strlen_gnl(savedbuff) - i + 1));
	if (!new_save)
		return (NULL);
	while (savedbuff[i])
		new_save[j++] = savedbuff[i++];
	new_save[j] = '\0';
	free(savedbuff);
	return (new_save);
}

char	*get_next_line(int fd)
{
	static char	*savedbuff = NULL;
	char		*line;
	char		*buff;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!savedbuff)
		savedbuff = ft_strdup_gnl("");
	buff = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buff)
	{
		free(savedbuff);
		return (NULL);
	}
	savedbuff = read_buff(buff, savedbuff, fd);
	if (!savedbuff)
	{
		free(savedbuff);
		return (NULL);
	}
	line = next_line(savedbuff);
	savedbuff = save(savedbuff);
	return (line);
}
