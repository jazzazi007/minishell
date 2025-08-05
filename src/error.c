/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 07:15:16 by ramroma           #+#    #+#             */
/*   Updated: 2025/08/05 20:23:09 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	is_syntax_error(t_tokenizer *tokens)
{
	if (!tokens)
		return false;
	if (tokens->type == T_PIPE || (tokens->type == T_PIPE && tokens->next->value == NULL )) //|| tokens->type == T_REDIR_OUT || tokens->type == T_REDIR_IN || tokens->type == T_APPEND ||
		// tokens->type == T_HEREDOC)
	{
		fprintf(stderr, "syntax error near unexpected token `%s'\n", tokens->value);
		return true;
	}
	while (tokens && tokens->next)
	{
		if ((tokens->type >= T_REDIR_IN && tokens->type <= T_HEREDOC)
			&& (tokens->next->type != T_WORD))
		{
			fprintf(stderr, "syntax error near unexpected token `%s'\n", tokens->next->value);
			return true;
		}
		tokens = tokens->next;
	}
	return false;
}

int ft_indicate_error(const char *msg, int exit_code, t_minishell *shell)
{
	if (msg)
		printf("%s\n", msg);
	shell->exit_status = exit_code;
	return (-1);
}
