/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 13:31:21 by ramroma           #+#    #+#             */
/*   Updated: 2025/08/16 15:39:52 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char *extract_unquoted_segment(const char *input, int *i)
{
	int start;
	char *token;
	
	start = *i;
	while (input[*i] && input[*i] != ' ')
	{
		if(is_metachar(input[*i]))
			break;
		(*i)++;
	}
	token = ft_substr(input, start, *i - start);
	if (!token)
		return (NULL);
	return (token);
}

static char *extract_word_combined(const char *input, int *i)
{
    char *word;
	char *part;
	char *tmp;
	
	word = ft_strdup("");
	if (input[*i] && input[*i] != ' ')
	{
		part = extract_unquoted_segment(input, i);
		if (!part)
		{
			free(word);
			return NULL;
		}
		tmp  = word;
		word = ft_strjoin(word, part);
		if (!word)
		{
			free(tmp);
			free(part);
			return NULL;
		}
		free(tmp);
		free(part);
	}
	return (word);
}

//static 
bool handle_metacharacters(const char *input, int *i, 
		t_tokenizer **head)
{
	char *new;
	
	if (!ft_strncmp(&input[*i], "<<", 2) || !ft_strncmp(&input[*i], ">>", 2))
	{
		new = ft_substr(input, *i, 2);
		if (!new)
			return (false);
		add_token(head, new_token(new, get_metatype(&input[*i])));
		*i += 2;
		free(new);
		return true;
	}
	else if (is_metachar(input[*i]))
	{
		new = ft_substr(input, *i, 1);
		if (!new)
			return (false);
		add_token(head, new_token(new, get_metatype(&input[*i])));
		(*i)++;
		free(new);
		return true;
	}
	return false;
}

t_tokenizer *tokenize_input(const char *input, t_minishell *shell)
{
	int i;
	t_tokenizer *head;
	char *word;
	
	head = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] == ' ')
			i++;
		if (!input[i])
			break;
		word = extract_word_combined(input, &i);
		if (!word)
		{
			ft_indicate_error("Memory allocation failed", 1, shell);
			return NULL;
		}
		if (ft_strcmp(word, "") != 0)
			add_token(&head, new_token(word, T_WORD));
		free(word);
		if (handle_metacharacters(input, &i, &head))
			continue;
	}
	return head;
}
