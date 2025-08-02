/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 07:01:45 by ramroma           #+#    #+#             */
/*   Updated: 2025/08/02 14:34:45 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool is_metachar(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

t_tokentype get_metatype(const char *s)
{
	if (!ft_strncmp(s, "<<", 2))
		return T_HEREDOC;
	if (!ft_strncmp(s, ">>", 2))
		return T_APPEND;
	if (*s == '<')
		return T_REDIR_IN;
	if (*s == '>')
		return T_REDIR_OUT;
	if (*s == '|')
		return T_PIPE;
	return T_WORD;
}

t_tokenizer	*new_token(const char *val, t_tokentype type)
{
	t_tokenizer	*tok;

	tok = malloc(sizeof(t_tokenizer));
	if (!tok)
		return (NULL);
	tok->value = ft_strdup(val);
	if (!tok->value)
	{
		free(tok);
		return (NULL);
	}
	tok->type = type;
	tok->in_single_quotes = 0;
	tok->in_double_quotes = 0;
	tok->next = NULL;
	return (tok);
}

void	add_token(t_tokenizer **head, t_tokenizer *new)
{
	t_tokenizer	*tmp;

	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void	free_tokens(t_tokenizer *head)
{
	t_tokenizer	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->value);
		free(tmp);
	}
}

