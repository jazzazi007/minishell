/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:00:44 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/05/07 18:26:54 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char	*input;
	setup_signal_handlers();
	
	while (1)
	{
		// setup_signal_handlers();
	
		input = readline("\001\033[31m\002minishell 🔥 > \001\033[0m\002");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		// if (g_exit_status == 130)
		// {
		// 	// printf("fjjf\n");
		// 	g_exit_status = 0;
		// 	// exit (130);
		// }
		if (*input)
			add_history(input);
		if (ft_strcmp(input, "exit") == 0)
		{
			free(input);
			break ;
		}
		free(input);
	}
	return (0);
}
