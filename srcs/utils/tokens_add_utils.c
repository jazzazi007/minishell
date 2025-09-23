/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_add_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felayan <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 21:13:23 by felayan           #+#    #+#             */
/*   Updated: 2025/09/22 22:49:28 by felayan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tk	get_opertype(const char *s)
{
	if (!ft_strncmp(s, "<<", 2))
		return (T_HEREDOC);
	if (!ft_strncmp(s, ">>", 2))
		return (T_APPEND);
	if (*s == '<')
		return (T_REDIR_IN);
	if (*s == '>')
		return (T_REDIR_OUT);
	if (*s == '|')
		return (T_PIPE);
	return (T_WORD);
}

static t_tokens	*get_last_token(t_tokens *tokens)
{
	if (!tokens)
		return (NULL);
	while (tokens -> next)
		tokens = tokens -> next;
	return (tokens);
}

void	add_token(t_shell *sh, t_tk t_type, char *token, bool exp)
{
	t_tokens	*new;
	t_tokens	*last;

	last = NULL;
	new = malloc(sizeof(t_tokens));
	if (!new)
	{
		free(token);
		clean_shell(sh, MALLOC_FAILURE);
	}
	new -> value = token;
	new -> type = t_type;
	new -> is_expandable = exp;
	new -> next = NULL;
	if (!sh -> tokens)
		sh -> tokens = new;
	else
	{
		last = get_last_token(sh -> tokens);
		last -> next = new;
	}
}
