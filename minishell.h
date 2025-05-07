/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:28:03 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/05/07 17:42:11 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include "./libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>

extern int g_exit_status;

void sigint_handler(int signum);
void setup_signal_handlers(void);

#endif