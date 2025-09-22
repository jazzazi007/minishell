/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felayan <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 21:12:40 by felayan           #+#    #+#             */
/*   Updated: 2025/09/21 21:12:41 by felayan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_var(char var)
{
	if (ft_isalnum(var) || var == '_' || var == '?')
		return (true);
	return (false);
}

char	*get_env_value(const char *key, char **envp)
{
	int		i;
	size_t	len;
	char	*tmp;

	i = 0;
	len = ft_strlen(key);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
		{
			tmp = ft_strdup(&envp[i][len + 1]);
			return (tmp);
		}
		i++;
	}
	tmp = ft_strdup("");
	return (tmp);
}

char	*append_char(char *s, char c)
{
	char	*appended;
	int		len;

	if (!s)
	{
		appended = malloc(2);
		if (!appended)
			return (NULL);
		appended[0] = c;
		appended[1] = '\0';
		return (appended);
	}
	len = ft_strlen(s);
	appended = malloc(len + 2);
	if (!appended)
	{
		free(s);
		return (NULL);
	}
	ft_memcpy(appended, s, len);
	appended[len++] = c;
	appended[len] = '\0';
	free(s);
	return (appended);
}

char	*append_str(char **env, char *s1, const char *s2, bool is_special)
{
	char	*val;
	char	*appended;

	val = NULL;
	appended = NULL;
	if (is_special)
		appended = ft_strjoin(s1, s2);
	else
	{
		val = get_env_value(s2, env);
		if (!val)
		{
			free(s1);
			return (NULL);
		}
		appended = ft_strjoin(s1, val);
		free(val);
	}
	free(s1);
	return (appended);
}

int	crt_var(char **env, char **expanded, char *key, bool is_special)
{
	if (!key)
	{
		free(*expanded);
		return (MALLOC_FAILURE);
	}
	*expanded = append_str(env, *expanded, key, is_special);
	free(key);
	if (!*expanded)
		return (MALLOC_FAILURE);
	return (SUCCESS);
}
