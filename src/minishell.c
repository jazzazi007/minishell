/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:00:44 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/08/03 17:27:40 by ralbliwi         ###   ########.fr       */
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
	t_tokenizer *tokens;

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
		if (!tokens || is_syntax_error(tokens))
		{
			free_tokens(tokens);
			free(input);
			continue;
		}
		// Debug print
		t_tokenizer *tmp = tokens;
		while (tmp)
		{
			printf("Token: %-10s | Type: %d\n", tmp->value, tmp->type);
			tmp = tmp->next;
		}
		expand_tokens(tokens, shell);
		printf("expansion part:\n");
		tmp = tokens;
		while (tmp)
		{
			printf("Token: %-10s | Type: %d\n", tmp->value, tmp->type);
			tmp = tmp->next;
		}
		printf("calling build cmd\n");
		shell->cmds = build_cmd(&tokens);
		free_tokens(tokens);
		if (!shell->cmds)
		{
			// free_split(cmd); //create a function
			shell->last_exit = 1;
			//ensure no leaks
			break;
		}
		ft_print_cmd(&shell->cmds);
		// For now, still use the old execution function
		// check_pipes_forks(input, shell);

	// 	// Clean up
	// 	free_tokens(tokens);
	// 	free(input);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_minishell *shell;
	int status;
	
	(void)ac;
	(void)av;
	shell = ft_init_shell(envp);
	if (!shell)
		return (1);
	ft_run_shell(shell);
	status = shell->last_exit;
	// ft_del_shell(shell); (TODO)
	// printf("exit\n");
	return (status);
}
