/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramroma <ramroma@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:00:28 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/05/28 17:51:52 by ramroma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int g_exit_status = 0; // Global variable to track exit status
// Handler for Ctrl+C (SIGINT)
void sigint_handler(int signum)
{
    (void)signum;
    write(1, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    g_exit_status = 130;
}
// Setup handlers for SIGINT and SIGQUIT

void setup_signal_handlers(void)
{
    struct sigaction sa;

    // Set handler for SIGINT (Ctrl+C)
    sa.sa_handler = sigint_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    // Ignore SIGQUIT (Ctrl-\)
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGQUIT, &sa, NULL);
}