/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 18:14:56 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/08/08 15:52:23 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	update_quotes(char c, int *squote, int *dquote)
{
	if (c == '\'' && !(*dquote))
		*squote += 1;
	else if (c == '"' && !(*squote))
		*dquote += 1;
}

char	*append_expanded_part(char *res, char *input, int *i,
	t_minishell *sh, int in_single)
{
	char	*tmp;

	if (input[*i] == '$' && !in_single)
	{
		(*i)++;
		tmp = expand_var(input, i, sh);
		if (!tmp)
			return (NULL);
		res = ft_strappend(res, tmp);
		if (!res)
		{
			free(tmp);
			return (NULL);
		}
		free(tmp);
	}
	else
	{
		tmp = ft_substr(input, *i, 1);
		if (!tmp)
			return (NULL);
		res = ft_strappend(res, tmp);
		if (!res)
		{
			free(tmp);
			return (NULL);
		}
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
		{
			res = append_expanded_part(res, input, &i, sh, in_squote);
			if (!res)
			{
				sh->exit_status = 1;
				return (NULL);
			}
		}	
	}
	return (res);
}

int	expand_tokens(t_tokenizer **head_r, t_minishell *sh)
{
	char	*expanded;
	t_tokenizer *head;

	if (!head_r || !*head_r || !sh)
		return (-1);
	head = *head_r;
	while (head)
	{
		if (head->type >= T_REDIR_IN && head->type < T_FILE)
			head->next->type = T_FILE;
		if (head->type == T_WORD)
		{
			expanded = expand_token_value(head->value, sh);
			free(head->value);
			head->value = expanded;
		}
		head = head->next;
	}
	return (0);
}
