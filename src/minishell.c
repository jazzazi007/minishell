/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:00:44 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/08/18 19:59:56 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void print_shell_banner(void)
{
    printf("\n");
    printf("\033[1;34m    ███████╗██╗  ██╗███████╗██╗     ██╗     ██╗  ██╗\n");
    printf("\033[1;34m    ██╔════╝██║  ██║██╔════╝██║     ██║     ╚██╗██╔╝\n");
    printf("\033[1;34m    ███████╗███████║█████╗  ██║     ██║      ╚███╔╝ \n");
    printf("\033[1;34m    ╚════██║██╔══██║██╔══╝  ██║     ██║      ██╔██╗ \n");
    printf("\033[1;34m    ███████║██║  ██║███████╗███████╗███████╗██╔╝ ██╗\n");
    printf("\033[1;34m    ╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝╚═╝  ╚═╝\n");
    printf("\033[0m\n");
}

void	ft_run_shell(t_minishell *shell)
{
	char	*input;
	
	setup_signal_handlers();
	while (1)
	{
		input = readline("\001\033[31m\002SHELLX 🔥 > \001\033[0m\002");
		if (!input)
			break;
		if(!*input)
		{
			free(input);
			continue;
		}
		add_history(input);
		if (ft_parse_cmd(shell, input) == -1)
		{
			free(input);
			continue;
		}
		free(input);
		check_pipes_forks(shell);
		// ft_free_args(shell->cmds->args);
		ft_free_cmds(shell->cmds);
		shell->cmds = NULL; 
	}
}

int	main(int ac, char **av, char **envp)
{
	t_minishell *shell;
	int status;
	
	(void)ac;
	(void)av;
	if(!&isatty)
		dup2(2, 0);
	print_shell_banner();
	shell = ft_init_shell(envp);
	if (!shell)
		return (1);
	ft_run_shell(shell);
	status = shell->exit_status;
	ft_free_args(shell->envp);
	ft_free_shell(shell);
	printf("exit\n");
	return (status);
}
	