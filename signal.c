/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:00:28 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/05/07 18:07:03 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_exit_status = 0;
//ctrl+c
// void sigint_handler(int signum)
// {
//     (void)signum;
//     write(1,"\n",1);
//     rl_replace_line("",0);
//     rl_on_new_line();
//     rl_redisplay();
//     g_exit_status = 130;
// }

// void setup_signal_handlers(void)
// {
//     struct sigaction sa;

//     sa.sa_handler = sigint_handler;  // set handler
//     sa.sa_flags = SA_RESTART;        // automatically restart interrupted syscalls
//     sigemptyset(&sa.sa_mask);        // clear mask so no signals are blocked

//     sigaction(SIGINT, &sa, NULL);
// }

void sigint_handler(int signum)
{
    (void)signum;
    write(1, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
    g_exit_status = 130;
}

void setup_signal_handlers(void)
{
    struct sigaction sa;

    sa.sa_handler = sigint_handler;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}