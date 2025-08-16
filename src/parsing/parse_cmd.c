/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:40:13 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/08/16 15:40:15 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int ft_parse_cmd(t_minishell *shell, const char *input)
{
    t_tokenizer *tokens;

    tokens = tokenize_input(input, shell);
	if (!tokens || is_syntax_error(&tokens, shell) == -1)
	{
		free_tokens(tokens);
		return (-1);
	}
	expand_tokens(&tokens, shell);
	shell->cmds = build_cmd(&tokens, shell);
	free_tokens(tokens);
	if (!shell->cmds || shell->cmds == NULL)
	{
		shell->exit_status = 1;
		return (-1);
	}
    return (0);
}
