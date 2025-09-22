/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 13:24:56 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/08/18 15:05:45 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_atoi_exit(const char *str, int *sta, bool *val)
{
	int	sign;
	size_t	i;

	sign = 1;
	i = skip_whitesp(str);
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i]  =='-')
			sign = -sign;
		i++;
	}
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		(*sta) = ((*sta) * 10) + (str[i] - '0');
		i++;
	}
	if (i != ft_strlen(str))
		i += skip_whitesp(&str[i]);
	if (i != ft_strlen(str))
	{
		(*sta) = 2;
		(*val) = false;
	}
	(*sta) *= sign;
}

static void	exit_status(t_shell *shell, int status, bool in_child)
{
	(void)status;
	if (!in_child)
		ft_putstr_fd("exit\n", 1);
	clean_shell(shell, shell -> exit_status);
}

void	exit_command(t_cmd *cmd, t_shell *shell)
{
	bool	valid;

	valid = true;
	if (shell -> cmd_count == 1 && !shell -> cmds -> args[1])
		exit_status(shell, shell -> exit_status, false);
	ft_atoi_exit(cmd -> args[1], &shell -> exit_status, &valid);
	if (shell -> exit_status == 2 && !valid)
	{
		if (shell -> cmd_count == 1)
			ft_putstr_fd("exit\n", 1);
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd -> args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit_status(shell, shell -> exit_status, true);
	}
	if (cmd->args[2])
	{
		if (shell -> cmd_count == 1)
			ft_putstr_fd("exit\n", 1);
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		shell -> exit_status = 1;
		return ;
	}
}

