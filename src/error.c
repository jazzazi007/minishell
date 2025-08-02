/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 07:15:16 by ramroma           #+#    #+#             */
/*   Updated: 2025/08/02 16:54:17 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	is_syntax_error(t_tokenizer *tokens)
{
	if (!tokens)
		return (false);
	if (tokens->type == T_PIPE || tokens->type == T_REDIR_OUT
		|| tokens->type == T_REDIR_IN || tokens->type == T_APPEND
		|| tokens->type == T_HEREDOC)
	{
		printf("syntax error near unexpected token `%s'\n", tokens->value);
		return (true);
	}
	while (tokens && tokens->next)
	{
		if ((tokens->type >= T_REDIR_IN && tokens->type <= T_HEREDOC)
			&& (tokens->next->type != T_WORD))
		{
			printf("syntax error near unexpected token `%s'\n",
				tokens->next->value);
			return (true);
		}
		tokens = tokens->next;
	}
	return (false);
}

int	check_unclosed_quotes(const char *input)
{
	int		i;
	int		in_squote;
	int		in_dquote;

	i = 0;
	in_squote = 0;
	in_dquote = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_dquote)
			in_squote ^= 1;
		else if (input[i] == '"' && !in_squote)
			in_dquote ^= 1;
		i++;
	}
	if (in_squote || in_dquote)
	{
		printf("syntax error: unclosed quote\n");
		return (258);
	}
	return (0);
}
