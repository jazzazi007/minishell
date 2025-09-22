/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felayan <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 14:22:01 by felayan           #+#    #+#             */
/*   Updated: 2025/09/22 14:22:02 by felayan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	token_cmd_count(t_tokens *tokens, int *wrds, int *rdrs)
{
	(*wrds) = 0;
	(*rdrs) = 0;
	while (tokens && !is_pipe(tokens -> value))
	{
		if (is_redir(tokens -> value))
		{
			(*rdrs)++;
			tokens = tokens -> next;
		}
		else
			(*wrds)++;
		tokens = tokens -> next;
	}
}

t_cmd	*init_cmd(t_shell *dt, t_tokens *tokens)
{
	t_cmd	*cmd;
	int		i;

	i = 0;
	(void)dt;
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		clean_shell(dt, MALLOC_FAILURE);
	ft_bzero(cmd, sizeof(t_cmd));
	token_cmd_count(tokens, &cmd -> word_count, &cmd -> redir_count);
	cmd -> args = malloc(sizeof(char *) * (cmd ->word_count + 1));
	if (cmd -> redir_count > 0)
		cmd -> redir = malloc(sizeof(t_redir) * (cmd -> redir_count));
	if (!cmd -> args || (!cmd -> redir && cmd ->redir_count > 0))
	{
		free(cmd -> args);
		free(cmd -> redir);
		free(cmd);
		clean_shell(dt, MALLOC_FAILURE);
	}
	while (i < cmd -> redir_count)
		cmd -> redir[i++].filename = NULL;
	cmd -> args[cmd -> word_count] = NULL;
	return (cmd);
}

t_cmd	*get_last_cmd(t_cmd *cmd)
{
	while (cmd && cmd -> next)
		cmd = cmd -> next;
	return (cmd);
}

int	add_redir_cmd(t_cmd *cmd, t_tokens **tokens, int	*rdr_i, t_shell *dt)
{
	if (!ft_strcmp((*tokens)-> value, ">"))
		cmd -> redir[*rdr_i].red_type = OUT;
	else if (!ft_strcmp((*tokens)-> value, "<"))
		cmd -> redir[*rdr_i].red_type = IN;
	else if (!ft_strcmp((*tokens)-> value, ">>"))
		cmd -> redir[*rdr_i].red_type = APPEND;
	else if (!ft_strcmp((*tokens)-> value, "<<"))
	{
		cmd -> redir[*rdr_i].red_type = HEREDOC;
		signal(SIGINT, SIG_IGN);
		setup_signal_handlers();
		cmd -> redir[*rdr_i].here_fd = ft_open_heredoc((*tokens)->next, dt);
		if (g_exit_status)
			return (g_exit_status);
	}
	(*tokens) = (*tokens)-> next;
	if (cmd -> redir[*rdr_i].red_type != HEREDOC)
	{
		cmd -> redir[*rdr_i].filename = ft_strdup((*tokens)-> value);
		if (!cmd -> redir[*rdr_i].filename)
			return (MALLOC_FAILURE);
	}
	(*rdr_i)++;
	return (SUCCESS);
}

int	add_word_cmd(t_cmd *cmd, const char *token, int *wrd_i)
{
	cmd -> args[*wrd_i] = ft_strdup(token);
	if (!cmd -> args[*wrd_i])
		return (MALLOC_FAILURE);
	(*wrd_i)++;
	return (SUCCESS);
}
