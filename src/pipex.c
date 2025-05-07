/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moaljazz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 19:57:50 by moaljazz          #+#    #+#             */
/*   Updated: 2025/01/05 19:57:54 by moaljazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	first_fork_operate(int infile, char **ag, char **env, int *pd)
{
	close(pd[0]);
	dup2(infile, STDIN_FILENO);
	close(infile);
	dup2(pd[1], STDOUT_FILENO);
	close(pd[1]);
	exit(cmd_exec(ag[2], env));
	close(infile);
}

void	second_fork_operate(int outfile, char **ag, char **env, int *pd)
{
	close(pd[1]);
	dup2(outfile, STDOUT_FILENO);
	close(outfile);
	dup2(pd[0], STDIN_FILENO);
	close(pd[0]);
	exit(cmd_exec(ag[3], env));
	close(outfile);
}

int	file_errhandle(int file)
{
	if (file < 0)
	{
		perror("Error");
		return (errno);
	}
	return (0);
}

void	pipex(int infile, int outfile, char **ag, char **env)
{
	pid_t	id;
	pid_t	id2;
	int		pd[2];

	if (pipe(pd) == -1)
		return ;
	id = fork();
	if (check_fork(id) < 0)
		return ;
	if (id == 0)
	{
		file_close(outfile);
		fork1(infile, ag, env, pd);
	}
	else
	{
		file_close(infile);
		id2 = fork();
		if (check_fork(id2) < 0)
			return ;
		if (id2 == 0)
			fork2(outfile, ag, env, pd);
		exit(close_pipes(outfile, pd, id, id2));
	}
}

int	main(int ac, char **av, char **envp)
{
	int	fd1;
	int	fd2;

	if (ac == 5)
	{
		fd1 = open(av[1], O_RDONLY);
		fd2 = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		pipex(fd1, fd2, av, envp);
		if (fd1 >= 0)
			close(fd1);
		if (fd2 >= 0)
			close(fd2);
	}
	else
	{
		write(1, "Error: invalid number of args\n", 31);
		exit(1);
	}
	return (0);
}
