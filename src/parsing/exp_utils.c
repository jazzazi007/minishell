/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 11:26:27 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/08/05 11:54:41 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_env_value(const char *key, char **envp)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(key);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
			return (&envp[i][len + 1]);
		i++;
	}
	return (NULL);
}

char	*expand_var(const char *str, int *i, t_minishell *sh)
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
	val = ft_strdup(get_env_value(key, sh->envp));
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
	if (access(cmd0, X_OK) == 0)
		return (ft_strdup(cmd0));
	return (get_cmd_path(cmd0, shell->envp));
}