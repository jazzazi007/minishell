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
#include "minishell.h"

static void	merge_tokens_exp(t_shell *dt, const char *line)
{
	t_tokenizer *tmp;
	t_tokenizer *to_free;
	char *merge;
	char	*chr;

	tmp = dt -> tokens -> next;
	while(tmp && tmp -> next)
	{
		chr = ft_strchr(line, '=');
			if (tmp -> value[ft_strlen(tmp -> value) - 1] == '='
				&& ((chr[1] == '\'' || chr[1] == '\"')))
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
			else
				tmp = tmp -> next;
			chr++;
	}
}

int parsing(t_shell *shell, const char *input)
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
			merge_tokens_exp(shell, input);
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
