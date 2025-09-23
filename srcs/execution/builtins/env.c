/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felayan <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:42:41 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/09/22 22:47:53 by felayan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env(t_shell *shell, t_cmd *cmd)
{
	int		i;
	char	**env;

	i = 0;
	env = shell -> envp;
	if (cmd -> args[1])
	{
		ft_putstr_fd("env: ", STDERR_FILENO);
		ft_putstr_fd(cmd -> args[1], STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		shell -> exit = 127;
	}
	else if (env)
	{
		while (env[i])
		{
			if (ft_strchr(env[i], '='))
			{
				ft_putstr_fd(env[i], STDOUT_FILENO);
				ft_putchar_fd('\n', STDOUT_FILENO);
			}
			i++;
		}
	}
	return (0);
}
