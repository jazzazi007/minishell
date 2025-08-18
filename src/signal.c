/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:00:28 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/08/18 20:07:09 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int g_exit_status = 0;

void sigint_handler(int signum)
{
    (void)signum;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
    g_exit_status = 130;
}

void setup_signal_handlers(void)
{
    struct sigaction sa;

    sa.sa_handler = sigint_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGQUIT, &sa, NULL);
}
void    signal_excuter(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}
void    handle_quit(int sig)
{
    (void)sig;
    write(1, "Quit (core dumped)\n", 19);
    signal(SIGQUIT, SIG_IGN);
}

void    handle_c(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    signal(SIGINT, SIG_IGN);
}
void    signla_exc_parent(void)
{    
    signal(SIGINT, handle_c);
    signal(SIGQUIT, handle_quit);
}
void    handle_herdoc(void)
{
    signal(SIGINT, SIG_IGN);
    g_exit_status = 130;
    close(0);
    // signal(SIGINT, SIG_DFL);
    // signal(SIGINT, )
}