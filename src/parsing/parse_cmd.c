/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:40:13 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/08/16 16:42:10 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_is_closed(t_tokenizer *tokens)
{
    t_tokenizer *tmp = tokens;
    int single_quote = 0;
    int double_quote = 0;
    int i;

    while (tmp)
    {
        i = 0;
        while (tmp->value[i])
        {
            if (tmp->value[i] == '\'')
                single_quote++;
            else if (tmp->value[i] == '\"')
                double_quote++;
            i++;
        }
        tmp = tmp->next;
    }
    if ((single_quote % 2 != 0) || (double_quote % 2 != 0))
    {
        write(2, "Unclosed quotes\n", 16);
        return (1);
    }
    return (0);
}

int ft_parse_cmd(t_minishell *shell, const char *input)
{
    t_tokenizer *tokens;

    tokens = tokenize_input(input, shell);
	if (!tokens || is_syntax_error(&tokens, shell) == -1)
	{
		free_tokens(tokens);
		return (-1);
	}
	if (check_is_closed(tokens))
	{
		shell->exit_status = 1;//check the exit status for unclosed quote commands
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
