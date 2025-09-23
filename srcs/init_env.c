/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felayan <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 21:17:22 by felayan           #+#    #+#             */
/*   Updated: 2025/09/23 00:39:04 by felayan          ###   ########.fr       */
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

static int	copy_env(char **dest, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		dest[i] = ft_strdup(envp[i]);
		if (!dest[i])
			return (MALLOC_FAILURE);
		i++;
	}
	dest[i] = NULL;
	return (SUCCESS);
}

void	init_env(t_shell *sh, char **env)
{
	int	entries;

	entries = count_env_entries(env);
	sh -> envp = malloc(sizeof(char *) * (entries + 1));
	if (!sh -> envp)
		clean_shell(sh, MALLOC_FAILURE);
	if (copy_env(sh -> envp, env))
		clean_shell(sh, MALLOC_FAILURE);
}
