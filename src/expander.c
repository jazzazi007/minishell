/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 18:14:56 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/08/03 11:27:49 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	update_quotes(char c, int *squote, int *dquote)
{
	if (c == '\'' && !(*dquote))
		*squote += 1;
	else if (c == '"' && !(*squote))
		*dquote += 1;
}

static char	*append_expanded_part(char *res, char *input, int *i,
	t_minishell *sh, int in_single)
{
	char	*tmp;

	if (input[*i] == '$' && !in_single)
	{
		(*i)++;
		tmp = expand_var(input, i, sh);
		res = ft_strappend(res, tmp);
		free(tmp);
	}
	else
	{
		tmp = ft_substr(input, *i, 1);
		res = ft_strappend(res, tmp);
		free(tmp);
		(*i)++;
	}
	return (res);
}

char	*expand_token_value(char *input, t_minishell *sh)
{
	char	*res;
	int		i;
	int		in_squote;
	int		in_dquote;

	res = ft_strdup("");
	i = 0;
	in_squote = 0;
	in_dquote = 0;
	while (input[i])
	{
		update_quotes(input[i], &in_squote, &in_dquote);
		if ((input[i] == '\'' && !in_dquote)
			|| (input[i] == '"' && !in_squote))
			i++;
		else
			res = append_expanded_part(res, input, &i, sh, in_squote);
	}
	return (res);
}

void	expand_tokens(t_tokenizer *head, t_minishell *sh)
{
	char	*expanded;

	while (head)
	{
		if (head->type == T_WORD)
		{
			expanded = expand_token_value(head->value, sh);
			free(head->value);
			head->value = expanded;
		}
		head = head->next;
	}
}
