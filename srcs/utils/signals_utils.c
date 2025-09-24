/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felayan <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 01:20:35 by felayan           #+#    #+#             */
/*   Updated: 2025/09/24 18:47:04 by felayan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_quit(int sig)
{
	(void)sig;
	write(1, "Quit (core dumped)\n", 19);
}

void	handle_c(int sig)
{
	(void)sig;
	write(1, "\n", 1);
}

void	handle_herdoc_c(int sig)
{
	(void)sig;
	g_exit_status = 130;
	write(1, "\n", 1);
	close(STDIN_FILENO);
}

void	sigint_handler(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_status = 130;
}
