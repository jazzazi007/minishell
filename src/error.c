/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 07:15:16 by ramroma           #+#    #+#             */
/*   Updated: 2025/08/16 16:03:42 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_syntax_error(t_tokenizer **r_tokens, t_minishell *shell)
{
	t_tokenizer *curr;
	
	curr = *r_tokens;
	if (curr->type == T_PIPE )
	{
		printf("syntax error near unexpected token `%s'\n", curr->value);
		return (ft_indicate_error(NULL, 1, shell));
	}
	while (curr)
	{
		if ((curr->type >= T_REDIR_IN && curr->type <= T_APPEND))
		{
			if (!curr->next || (curr->next->type != T_WORD && curr->next->type != T_FILE))
			{
				printf("syntax error near unexpected token `%s'\n", curr->value);
				return (ft_indicate_error(NULL, 1, shell));
			}
		}
		if (curr->type == T_PIPE && (!curr->next))
		{
			printf("syntax error near unexpected token `%s'\n", curr->value);
			return (ft_indicate_error(NULL, 1, shell));
		}
		curr = curr->next;
	}
	return (0);
}

int ft_indicate_error(const char *msg, int exit_code, t_minishell *shell)
{
	if (msg)
		printf("%s\n", msg);
	shell->exit_status = exit_code;
	return (-1);
}
