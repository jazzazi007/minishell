/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 13:31:21 by ramroma           #+#    #+#             */
/*   Updated: 2025/08/08 11:38:01 by codespace        ###   ########.fr       */
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
	if (!ft_strncmp(&input[*i], "<<", 2) || !ft_strncmp(&input[*i], ">>", 2))
	{
		add_token(head, new_token(ft_substr(input, *i, 2), //leak
			get_metatype(&input[*i])));
		*i += 2;
		return true;
	}
	else if (is_metachar(input[*i]))
	{
		add_token(head, new_token(ft_substr(input, *i, 1), 
			get_metatype(&input[*i])));
		(*i)++;
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
		add_token(&head, new_token(word, T_WORD));
		free(word);
		if (handle_metacharacters(input, &i, &head))
			continue;
	}
	return head;
}

// t_tokenizer *tokenize_input(const char *input)
// {
// 	int i;
// 	t_tokenizer *head;
// 	char *word;

// 	head = NULL;
// 	i = 0;
// 	while (input[i])
// 	{
// 		while (input[i] == ' ')
// 			i++;
// 		if (!input[i])
// 			break;
// 		if (handle_metacharacters(input, &i, &head))
// 			continue;
// 		word = extract_word_combined(input, &i);
// 		printf("Extracted word: %s\n", word);
// 		add_token(&head, new_token(word, T_WORD));
// 		free(word);
// 	}
// 	return head;
// }