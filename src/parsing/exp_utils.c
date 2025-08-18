/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 11:26:27 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/08/18 16:56:47 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_env_value(const char *key, char **envp)
{
	int		i;
	size_t	len;
	char *tmp;

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

char	*ft_var_expand(const char *str, int *i, t_minishell *sh)
{
	char	*key;
	char	*val;
	int		start;

	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(sh->exit_status));
	}
	if (!ft_isalnum(str[*i]) && str[*i] != '_')
		return (ft_strdup("$"));
	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	key = ft_substr(str, start, *i - start);
	if (!key)
		return (NULL);
	val = get_env_value(key, sh->envp);
	free(key);
	return (val);
}

char	*ft_strappend(char *dst, const char *src)
{
	char	*tmp;
	size_t	len;

	if (!src)
		return (dst);
	len = ft_strlen(dst) + ft_strlen(src);
	tmp = malloc(len + 1);
	if (!tmp)
		return (NULL);
	tmp[0] = '\0';
	if (dst)
	{
		ft_strlcpy(tmp, dst, len + 1);
		free(dst);
	}
	ft_strlcat(tmp, src, len + 1);
	return (tmp);
}

char	*resolve_cmd_path(char *cmd0, t_minishell *shell)
{
	if (!cmd0)
		return(NULL);
	if (access(cmd0, X_OK) == 0)
		return (ft_strdup(cmd0));
	return (get_cmd_path(cmd0, shell->envp));
}

void ft_strstrip(char **str_r)
{
	int i;
	int j;
	char quote;
	char *buff;

	i = 0;
	j = 0;
	quote = '\0';
	buff = *str_r;
	while ((*str_r)[i])
	{
		if (((*str_r)[i] == '\'' || (*str_r)[i] == '\"') && quote == '\0')
		{
			quote = (*str_r)[i];
			i++;
		}
		if ((*str_r)[i] && ((*str_r)[i] != quote))
		{
			buff[j] = (*str_r)[i];
			i++;
			j++;
		}
		if (quote && (*str_r)[i] == quote)
		{
			quote = '\0';
			i++;
		}
	}
	buff[j] = '\0';
	*str_r = buff;
}
