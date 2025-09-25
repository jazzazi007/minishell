/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felayan <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:57:43 by felayan           #+#    #+#             */
/*   Updated: 2025/09/25 17:17:34 by felayan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_env_entries(char **envp)
{
	int	entries;

	entries = 0;
	while (envp[entries])
		entries++;
	return (entries);
}

char	**get_env_key(t_shell *sh, const char *key)
{
	int		i;
	int		len;
	char	*eq;

	i = 0;
	eq = ft_strchr(key, '=');
	if (eq)
		len = eq - key;
	else
		len = ft_strlen(key);
	while (sh -> envp[i])
	{
		if (!ft_strncmp(sh -> envp[i], key, len)
			&& (sh -> envp[i][len] == '\0' || sh -> envp[i][len] == '='))
			return (&sh -> envp[i]);
		i++;
	}
	return (NULL);
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

void	update_shlvl(t_shell *sh, char **shlvl)
{
	char	*num;
	char	*new_val;
	char	**new_env;

	shlvl = get_env_key(sh, "SHLVL");
	if (!shlvl || !ft_strchr(*shlvl, '='))
	{
		if (update_env(sh, "SHLVL=1", &new_env))
			clean_strs(new_env);
		return ;
	}
	if ((*shlvl)[ft_strchr(*shlvl, '=') - *shlvl + 1] == '\0')
		update_key(shlvl, "SHLVL=1");
	else
	{
		num = ft_itoa(ft_atoi(ft_strchr(*shlvl, '=') + 1) + 1);
		if (!num)
			return ;
		new_val = ft_strjoin("SHLVL=", num);
		free(num);
		if (!new_val)
			return ;
		update_key(shlvl, new_val);
		free(new_val);
	}
}
