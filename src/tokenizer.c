/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 13:31:21 by ramroma           #+#    #+#             */
/*   Updated: 2025/07/19 11:29:30 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char *extract_quoted_segment(const char *input, int *i)
{
	char quote = input[(*i)++];
	int start = *i;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	char *part = ft_substr(input, start, *i - start);
	if (input[*i] == quote)
		(*i)++;
	return (part);
}

static char *extract_unquoted_segment(const char *input, int *i)
{
	int start = *i;
	while (input[*i] && input[*i] != ' ' && !is_metachar(input[*i]) &&
			input[*i] != '\"' && input[*i] != '\'')
		(*i)++;
	return ft_substr(input, start, *i - start);
}
static char *extract_word_combined(const char *input, int *i)
{
    char *word = ft_strdup("");
	while (input[*i] && input[*i] != ' ' && !is_metachar(input[*i]))
    {
        char *part = NULL;
        if (input[*i] == '\"' || input[*i] == '\'')
            part = extract_quoted_segment(input, i);
		else
			part = extract_unquoted_segment(input, i);
		char *tmp;

        tmp  = word;
		word = ft_strjoin(word, part);
		free(tmp);
		free(part);
	}
    return (word);
}
static bool handle_metacharacters(const char *input, int *i, t_tokenizer **head)
{
	if (!ft_strncmp(&input[*i], "<<", 2) || !ft_strncmp(&input[*i], ">>", 2))
	{
		add_token(head, new_token(ft_substr(input, *i, 2), get_metatype(&input[*i])));
		*i += 2;
		return true;
	}
	else if (is_metachar(input[*i]))
	{
		add_token(head, new_token(ft_substr(input, *i, 1), get_metatype(&input[*i])));
		(*i)++;
		return true;
	}
	return false;
}

t_tokenizer *tokenize_input(const char *input)
{
	int i;
	t_tokenizer *head;

    head = NULL;
    i = 0;
	while (input[i])
	{
		while (input[i] == ' ')
			i++;
		if (!input[i])
			break;
		if (handle_metacharacters(input, &i, &head))
			continue;
		char *word;
        word = extract_word_combined(input, &i);
		add_token(&head, new_token(word, T_WORD));
		free(word);
	}
	return head;
}
