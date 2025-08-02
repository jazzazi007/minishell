/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:00:44 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/08/02 19:40:07 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void print_shell_banner(void)
{
	write(STDOUT_FILENO, "\033[2J\033[H", 7);
    printf("\n");
    printf("\033[1;34m    ███████╗██╗  ██╗███████╗██╗     ██╗     ██╗  ██╗\n");
    printf("\033[1;34m    ██╔════╝██║  ██║██╔════╝██║     ██║     ╚██╗██╔╝\n");
    printf("\033[1;34m    ███████╗███████║█████╗  ██║     ██║      ╚███╔╝ \n");
    printf("\033[1;34m    ╚════██║██╔══██║██╔══╝  ██║     ██║      ██╔██╗ \n");
    printf("\033[1;34m    ███████║██║  ██║███████╗███████╗███████╗██╔╝ ██╗\n");
    printf("\033[1;34m    ╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝╚═╝  ╚═╝\n");
    printf("\033[0m\n");
}

static void	handle_input(char *input, t_minishell *sh)
{
	t_tokenizer	*tokens;

	check_unclosed_quotes(input);
	tokens = tokenize_input(input);
	if (!tokens)
		return ;
	if (is_syntax_error(tokens))
	{
		printf("here");
		sh->last_exit = 258;
		free_tokens(tokens);
		return ;
	}
	// expand_tokens(tokens, sh);
	cmd_exec(input , sh);
	// check_pipes_forks(input, sh->envp);
	free_tokens(tokens);
}

int	main(int ac, char **av, char **envp)
{
	t_minishell	sh;
	char		*input;

	(void)ac;
	(void)av;
	sh.envp = envp;
	sh.last_exit = 0;
	setup_signal_handlers();
	print_shell_banner();
	while (1)
	{
		input = readline("\001\033[31m\002SHELLX 🔥 > \001\033[0m\002");
		if (!input)
			break ;
		if (*input)
			add_history(input);
		if (!ft_strcmp(input, "exit"))
		{
			free(input);
			break ;
		}
		handle_input(input, &sh);
		free(input);
	}
	printf("exit\n");
	return (0);
}

// int	main(int ac, char **av, char **envp)
// {
// 	(void)ac;
//     (void)av;
// 	char	*input;
// 	setup_signal_handlers();
// 	check_pipes_forks("clear", envp);
//     print_shell_banner();
// 	while (1)
// 	{
// 		input = readline("\001\033[31m\002SHELLX 🔥 > \001\033[0m\002");
// 		if (!input)
// 		{
// 			fprintf(stderr,"exit\n");
//             break;
// 		}
// 		if (*input)
// 			add_history(input);
// 		if (ft_strcmp(input, "exit") == 0)
// 		{
// 			free(input);
// 			break ;
// 		}
// 		check_pipes_forks(input, envp);
// 		free(input);
// 	}
// 	return (0);
// }
