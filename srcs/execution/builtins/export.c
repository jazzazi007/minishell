/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felayan <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 01:37:10 by felayan           #+#    #+#             */
/*   Updated: 2025/09/26 00:57:05 by felayan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_key(const char *name)
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

static void	print_exp(char **env)
{
	int		i;
	char	*eq;
	int		key_len;

	i = 0;
	while (env[i])
	{
		eq = ft_strchr(env[i], '=');
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		if (eq)
		{
			key_len = eq - env[i];
			write(1, env[i], key_len);
			printf("=\"%s\"\n", eq + 1);
		}
		else
			printf("%s\n", env[i]);
		i++;
	}
}

static void	export_add(char *arg, t_shell *sh)
{
	if (!is_valid_key(arg))
	{
		sh -> exit = 1;
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
	}
	else
	{
		set_var(sh, arg);
		sh -> exit = 0;
	}
}

int	export_cmd(char **args, t_shell *shell)
{
	int	i;

	i = 1;
	if (!args[1])
	{
		shell -> exit = 0;
		print_exp(shell -> envp);
	}
	while (args[i])
	{
		export_add(args[i], shell);
		i++;
	}
	return (SUCCESS);
}
