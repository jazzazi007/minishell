/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felayan <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 14:21:27 by felayan           #+#    #+#             */
/*   Updated: 2025/09/22 14:21:28 by felayan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_var_expand(const char *str, int *i, t_shell *sh)
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
		ft_strlcpy(tmp, dst, len + 1);
	ft_strlcat(tmp, src, len + 1);
	return (tmp);
}

char	*append_expanded_part(char *res, char *input, int *i, t_shell *sh)
{
	char	*tmp;
	char	*appn;

	tmp = NULL;
	if (input[*i] == '$')
	{
		(*i)++;
		tmp = ft_var_expand(input, i, sh);
		if (!tmp)
			return (NULL);
	}
	else
	{
		tmp = ft_substr(input, *i, 1);
		if (!tmp)
			return (NULL);
		(*i)++;
	}
	appn = ft_strappend(res, tmp);
	free(res);
	free(tmp);
	return (appn);
}
