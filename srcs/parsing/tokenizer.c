/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felayan <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 21:09:28 by felayan           #+#    #+#             */
/*   Updated: 2025/09/21 21:11:55 by felayan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	create_tokens(const char *input, t_shell *dt)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '\'')
			add_single_quotes(dt, input, &i);
		else if (input[i] == '\"')
			add_double_quotes(dt, input, &i);
		else if (is_oper(input[i]))
			add_operator(dt, input, &i);
		else if (is_word(&input[i]))
			add_word(dt, input, &i);
		else
			i++;
	}
}

int	tokenizer(const char *input, t_shell *shell)
{
	int		i;
	bool	is_closed;

	i = skip_whitesp(input);
	is_closed = is_closed_quotes(input, i);
	if (is_closed)
		create_tokens(input + i, shell);
	if (syntax_check(shell -> tokens, !is_closed))
	{
		shell -> exit_status = 2;
		return (SYNTAX_ERR);
	}
	return (SUCCESS);
}
