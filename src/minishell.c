/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:00:44 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/08/03 12:03:25 by ralbliwi         ###   ########.fr       */
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

int	ft_run_shell(t_minishell *shell)
{
	char	*input;
	t_tokenizer *tokens;
	// char		**cmd;

	setup_signal_handlers();
	// check_pipes_forks("clear", envp); // still uses old exec system
	print_shell_banner();
	
	while (1)
	{
		input = readline("\001\033[31m\002SHELLX 🔥 > \001\033[0m\002");
		if (!input)
		{
			fprintf(stderr, "exit\n");
			break;
		}
		if (*input)
			add_history(input);
		if (ft_strcmp(input, "exit") == 0)
		{
			free(input);
			break;
		}
		// Tokenize the input first
		tokens = tokenize_input(input);
		// Debug print
		t_tokenizer *tmp = tokens;
		while (tmp)
		{
			printf("Token: %-10s | Type: %d\n", tmp->value, tmp->type);
			tmp = tmp->next;
		}
		expand_tokens(tokens, shell);
	// 	cmd = build_argv(tokens);
	// 	free_tokens(tokens);
	// 	if (!cmd || !cmd[0])
	// 	{
	// 		free_split(cmd);
	// 		return (0);
	// 	}

	// 	// For now, still use the old execution function
	// 	// check_pipes_forks(input, envp);

	// 	// Clean up
	// 	free_tokens(tokens);
	// 	free(input);
	}
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	t_minishell *shell;
	
	(void)ac;
	(void)av;
	shell = ft_init_shell(envp);
	if (!shell)
		return (1);
	ft_run_shell(shell);
	printf("exit\n");
	return (0);
}
