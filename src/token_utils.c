/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramroma <ramroma@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 07:01:45 by ramroma           #+#    #+#             */
/*   Updated: 2025/07/18 07:01:49 by ramroma          ###   ########.fr       */
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

t_tokenizer *new_token(const char *val, t_tokentype type)
{
	t_tokenizer *tok = malloc(sizeof(t_tokenizer));
	if (!tok)
		return NULL;
	tok->value = ft_strdup(val);
	tok->type = type;
	tok->next = NULL;
	return tok;
}

void add_token(t_tokenizer **head, t_tokenizer *new)
{
	if (!*head)
		*head = new;
	else
	{
		t_tokenizer *tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void free_tokens(t_tokenizer *head)
{
	while (head)
	{
		t_tokenizer *tmp = head;
		head = head->next;
		free(tmp->value);
		free(tmp);
	}
}
