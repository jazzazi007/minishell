/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felayan <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 00:19:34 by felayan           #+#    #+#             */
/*   Updated: 2025/09/23 01:37:05 by felayan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_valid_key(const char *name)
{
	int	i;

	i = 1;
	if (name[0] == '_' || ft_isalpha(name[0]))
	{
		while (name[i] && name[i] != '=')
		{
			if (!(name[i] == '_' || ft_isalnum(name[i])))
				return (false);
			i++;
		}
		return (true);
	}
	return (false);
}

static char	**get_env_key(t_shell *sh, const char *key)
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

static int	update_env(t_shell *sh, const char *var, char ***new_env)
{
	int		i;

	i = 0;
	new_env[0] = malloc(sizeof(char *) * (count_env_entries(sh -> envp) + 2));
	if (!new_env[0])
		return (MALLOC_FAILURE);
	while (sh -> envp[i])
	{
		new_env[0][i] = ft_strdup(sh -> envp[i]);
		if (!new_env[0][i])
			return (MALLOC_FAILURE);
		i++;
	}
	new_env[0][i] = ft_strdup(var);
	if (!new_env[0][i])
		return (MALLOC_FAILURE);
	new_env[0][++i] = NULL;
	clean_strs(sh -> envp);
	sh -> envp = new_env[0];
	return (SUCCESS);
}

static void	update_key(char **key, const char *var)
{
	char	*tmp;

	tmp = ft_strdup(var);
	if (!tmp)
		return ;
	free(*key);
	*key = tmp;
}

void	set_var(t_shell *sh, const char *var)
{
	char	*eq;
	char	**key;
	char	**new_env;

	new_env = NULL;
	key = get_env_key(sh, var);
	eq = ft_strchr(var, '=');
	if (!key)
	{
		if (update_env(sh, var, &new_env))
			clean_strs(new_env);
	}
	else if (eq)
		update_key(key, var);
	return ;
}
