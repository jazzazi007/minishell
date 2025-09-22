/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felayan <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 21:09:57 by felayan           #+#    #+#             */
/*   Updated: 2025/09/21 22:06:45 by felayan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	append_cmd(t_shell *dt, t_cmd *new)
{
	t_cmd	*last;

	if (!dt -> cmds)
		dt -> cmds = new;
	else
	{
		last = get_last_cmd(dt -> cmds);
		last -> next = new;
	}
	dt -> cmd_count++;
}

static int	add_cmd(t_cmd *cmd, t_tokens **tokens, t_shell *dt)
{
	int	status;
	int	wrd_i;
	int	rdr_i;

	wrd_i = 0;
	rdr_i = 0;
	status = 0;
	while (*tokens && !is_pipe((*tokens)-> value))
	{
		if (is_redir((*tokens)-> value))
		{
			status = add_redir_cmd(cmd, tokens, &rdr_i, dt);
			if (status == MALLOC_FAILURE)
				return (MALLOC_FAILURE);
			else if (status == 130)
				return (130);
		}
		else
		{
			if (add_word_cmd(cmd, (*tokens)-> value, &wrd_i))
				return (MALLOC_FAILURE);
		}
		(*tokens) = (*tokens)-> next;
	}
	return (SUCCESS);
}

static int	tokens_to_cmd(t_shell *dt, t_tokens *tokens)
{
	t_cmd	*new_cmd;
	int		status;

	while (tokens)
	{
		new_cmd = init_cmd(dt, tokens);
		status = add_cmd(new_cmd, &tokens, dt);
		if (status == MALLOC_FAILURE)
		{
			clean_cmds(new_cmd);
			clean_shell(dt, MALLOC_FAILURE);
		}
		else if (status == 130)
		{
			clean_cmds(new_cmd);
			return (status);
		}
		append_cmd(dt, new_cmd);
		if (tokens && is_pipe(tokens -> value))
			tokens = tokens -> next;
	}
	return (SUCCESS);
}

static void	merge_tokens_exp(t_shell *dt, const char *line, t_tokens *tmp)
{
	t_tokens	*to_free;
	char		*merge;
	char		*eq;

	eq = ft_strchr(line, '=');
	while (tmp && tmp -> next && eq)
	{
		if (tmp -> value[ft_strlen(tmp -> value) - 1] == '=')
		{
			if (eq[1] == '\'' || eq[1] == '\"')
			{
				merge = ft_strjoin(tmp -> value, tmp -> next -> value);
				if (!merge)
					clean_shell(dt, MALLOC_FAILURE);
				free(tmp -> value);
				tmp -> value = merge;
				to_free = tmp -> next;
				tmp -> next = to_free -> next;
				free(to_free->value);
				free(to_free);
			}
			eq = ft_strchr(eq + 1, '=');
		}
		tmp = tmp -> next;
	}
}

int	parsing(t_shell *shell, const char *input)
{
	if (tokenizer(input, shell) == SYNTAX_ERR)
	{
		clean_tokens(shell -> tokens);
		shell -> tokens = NULL;
		return (SYNTAX_ERR);
	}
	expander(shell);
	if (!ft_strcmp(shell -> tokens -> value, "export")
		&& shell -> tokens -> next)
		merge_tokens_exp(shell, input, shell -> tokens -> next);
	if (tokens_to_cmd(shell, shell -> tokens) == 130)
	{
		clean_tokens(shell -> tokens);
		shell -> tokens = NULL;
		clean_cmds(shell -> cmds);
		shell -> cmds = NULL;
		return (130);
	}
	clean_tokens(shell -> tokens);
	shell -> tokens = NULL;
	return (SUCCESS);
}
