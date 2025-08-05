/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:23:24 by moaljazz          #+#    #+#             */
/*   Updated: 2025/08/05 17:59:15 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int ft_count_pipe(t_tokenizer **r_tokens)
{
	int count;
	t_tokenizer *curr;

	count = 0;
	curr = *r_tokens;
	while(curr->next != NULL)
	{
		if (ft_strncmp(curr->value, "|", 1) == 0)
			count++;
		curr = curr->next;
	}
	return (count);
}

int **init_pipes(int pipe_count)
{
    int **pipe_fds;
    int i;

    pipe_fds = malloc(sizeof(int *) * pipe_count);
    if (!pipe_fds)
        return (NULL);
    i = 0;
    while (i < pipe_count)
    {
        pipe_fds[i] = malloc(sizeof(int) * 2);
        if (!pipe_fds[i])
        {
            while (--i >= 0)
                free(pipe_fds[i]);
            free(pipe_fds);
            return (NULL);
        }
        if (pipe(pipe_fds[i]) == -1)
        {
            while (i >= 0)
                free(pipe_fds[i]);
            free(pipe_fds);
            return (NULL);
        }
        i++;
    }
    return (pipe_fds);
}

 pid_t *init_child_pids(int pipe_count, int **pipe_fds)
{
    pid_t *child_pids;

    child_pids = malloc(sizeof(pid_t) * (pipe_count + 1));
    if (!child_pids)
    {
        int i = 0;
        while (i < pipe_count)
        {
            free(pipe_fds[i]);
            i++;
        }
        free(pipe_fds);
        return (NULL);
    }
    return (child_pids);
}

void cleanup_resources(int **pipe_fds, pid_t *child_pids, int pipe_count)
{
    int i;

    i = 0;
    while (i < pipe_count)
    {
        close(pipe_fds[i][0]);
        close(pipe_fds[i][1]);
        i++;
    }

    i = 0;
    while (i <= pipe_count)
    {
        waitpid(child_pids[i], NULL, 0);
        i++;
    }

    i = 0;
    while (i < pipe_count)
    {
        free(pipe_fds[i]);
        i++;
    }
    free(pipe_fds);
    free(child_pids);
}
