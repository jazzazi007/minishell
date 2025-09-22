/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felayan <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 21:17:16 by felayan           #+#    #+#             */
/*   Updated: 2025/09/22 05:38:28 by felayan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_shell_banner(char **envp)
{
	char	*clear[2];
	pid_t	pid;

	clear[0] = "/usr/bin/clear";
	clear[1] = NULL;
	pid = fork();
	if (!pid)
	{
		execve(clear[0], clear, envp);
		perror("Banner execve failed");
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		waitpid(pid, NULL, 0);
		printf(BBLU"\n\t███████╗██╗  ██╗███████╗██╗     ██╗     ██╗  ██╗\n");
		printf("\t██╔════╝██║  ██║██╔════╝██║     ██║     ╚██╗██╔╝\n");
		printf("\t███████╗███████║█████╗  ██║     ██║      ╚███╔╝ \n");
		printf("\t╚════██║██╔══██║██╔══╝  ██║     ██║      ██╔██╗ \n");
		printf("\t███████║██║  ██║███████╗███████╗███████╗██╔╝ ██╗\n");
		printf("\t╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝╚═╝  ╚═╝\n"RST"\n");
	}
	else
		perror("fork banner");
}

static void	start_shell(t_shell *shell, char *line, char **envp)
{
	init_env(shell, envp);
	while (42)
	{
		shell -> cmd_count = 0;
		setup_signal_handlers();
		if (!isatty(STDIN_FILENO))
			dup2(STDERR_FILENO, STDIN_FILENO);
		line = readline("\001\033[31m\002SHELLX 🔥 > \001\033[0m\002");
		shell -> exit_status = g_exit_status;
		g_exit_status = 0;
		if (!line)
			break ;
		if (!is_empty(line))
		{
			add_history(line);
			if (!parsing(shell, line))
				execution(shell);
			free(line);
			clean_cmds(shell->cmds);
			shell->cmds = NULL;
		}
	}
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;
	char	*line;

	(void)ac;
	(void)av;
	line = NULL;
	print_shell_banner(envp);
	ft_bzero(&shell, sizeof(shell));
	start_shell(&shell, line, envp);
	printf("exit\n");
	close(0);
	clean_shell(&shell, shell.exit_status);
}
