/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felayan <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 21:17:22 by felayan           #+#    #+#             */
/*   Updated: 2025/09/21 21:17:23 by felayan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_env_entries(char **envp)
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

void	init_env(t_shell *dt, char **env)
{
	int	entries;

	entries = count_env_entries(env);
	dt -> envp = malloc(sizeof(char *) * (entries + 1));
	if (!dt -> envp)
		clean_shell(dt, MALLOC_FAILURE);
	if (copy_env(dt -> envp, env))
		clean_shell(dt, MALLOC_FAILURE);
}
