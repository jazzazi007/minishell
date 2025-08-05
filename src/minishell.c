/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:00:44 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/08/05 17:52:49 by codespace        ###   ########.fr       */
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

void	ft_run_shell(t_minishell **r_shell)
{
	char	*input;
	t_tokenizer *tokens;

	setup_signal_handlers();
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
		if (ft_strcmp(input, "exit") == 0) //replace this with exit command built-in
		{
			free(input);
			break;
		}
		tokens = tokenize_input(input);
		if (!tokens || is_syntax_error(tokens))
		{
			free_tokens(tokens);
			free(input);
			continue;
		}
		free(input);
		// Debug print
		// t_tokenizer *tmp = tokens;
		// while (tmp)
		// {
		// 	printf("Token: %-10s | Type: %d\n", tmp->value, tmp->type);
		// 	tmp = tmp->next;
		// }
		expand_tokens(tokens, *r_shell);
		(*r_shell)->cmds = build_cmd(&tokens);
		free_tokens(tokens);
		if (!(*r_shell)->cmds || (*r_shell)->cmds == NULL)
		{
			(*r_shell)->exit_status = 1;
			break;
		}
		ft_print_cmd(&(*r_shell)->cmds);
		// For now, still use the old execution function
		// check_pipes_forks(input, shell);
		ft_free_cmds((*r_shell)->cmds);
		(*r_shell)->cmds = NULL; // Prevent double free
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
	ft_run_shell(&shell);
	status = shell->exit_status;
	ft_free_shell(shell);
	return (status);
}
