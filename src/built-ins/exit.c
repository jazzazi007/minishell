/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 13:24:56 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/08/16 15:33:16 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int count_cmd(t_cmd *cmd) {
    int i;

    i = 0;
    if (!cmd)
        return 0;
    while (cmd) {
        i++;
        cmd = cmd->next;
    }
    return i;
}

static int	parse_sign(const char **str)
{
	int	sign;

	sign = 1;
	if (**str == '+' || **str == '-')
	{
		if (**str == '-')
			sign = -1;
		(*str)++;
	}
	return (sign);
}

static int	parse_digits(const char **str, int *found_digit)
{
	int	result;

	result = 0;
	while (**str >= '0' && **str <= '9')
	{
		*found_digit = 1;
		result = result * 10 + (**str - '0');
		(*str)++;
	}
	return (result);
}
const char	*skip_whitespace(const char *str)
{
	while (*str && ft_strchr(SPACES, *str))
		str++;
	return (str);
}

static int	ft_atoi_exit(const char *str, char **endptr)
{
	int	result;
	int	sign;
	int	found_digit;

	str = skip_whitespace(str);
	sign = parse_sign(&str);
	result = parse_digits(&str, &found_digit);
	if (!found_digit)
	{
		if (endptr)
			*endptr = (char *)str;
		return (0);
	}
	if (endptr)
		*endptr = (char *)str;
	return (result * sign);
}

static void	exit_status(t_minishell *shell, int status)
{
	ft_free_shell(shell);
	exit(status);
}

void exit_command(t_cmd *cmd, t_minishell *shell)
{
    int status;
    char *end;

    int count = count_cmd(cmd);
    if (count == 1)
        ft_putstr_fd("exit\n", 1);
    if (!cmd->args[1])
        exit_status(shell, 0);
    status = ft_atoi_exit(cmd->args[1], &end);
    if (*end != '\0')
    {
        ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
		exit_status(shell, 2);
    }
    if (cmd->args[2])
    {
        ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		shell->exit_status = 1;
		return ;
    }
    exit_status(shell, status);
}

