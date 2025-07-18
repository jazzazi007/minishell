/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramroma <ramroma@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 13:31:21 by ramroma           #+#    #+#             */
/*   Updated: 2025/07/18 11:00:25 by ramroma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// static char *extract_quoted(const char *src, int *i, char quote)
// {
//     int start = ++(*i);
//     while (src[*i] && (get_metatype(&src[*i])) == T_WORD)
//         (*i)++;
//     char *res = ft_substr(src, start, *i - start);
//     if (src[*i] == quote)
//         (*i)++;
//     return res;
// }

// static char *extract_word(const char *src, int *i)
// {
//     int start = *i;
//     while (src[*i] && !is_metachar(src[*i]) && src[*i] != ' ' && src[*i] != '\"' && src[*i] != '\'')
//         (*i)++;
//     return ft_substr(src, start, *i - start);
// }

// t_tokenizer *tokenize_input(const char *input)
// {
//     int i = 0;
//     t_tokenizer *head = NULL;

//     while (input[i])
//     {
//         while (input[i] == ' ')
//             i++;

//         if (!input[i])
//             break;

//         if (input[i] == '\"' || input[i] == '\'')
//         {
//             char quote = input[i];
//             char *word = extract_quoted(input, &i, quote); 
//             add_token(&head, new_token(word, T_WORD));
//             free(word);
//         }
//         else if (!ft_strncmp(&input[i], "<<", 2) || !ft_strncmp(&input[i], ">>", 2))
//         {
//             add_token(&head, new_token(ft_substr(input, i, 2), get_metatype(&input[i])));
//             i += 2;
//         }
//         else if (is_metachar(input[i]))
//         {
//             add_token(&head, new_token(ft_substr(input, i, 1), get_metatype(&input[i])));
//             i++;
//         }
//         else
//         {
//             char *word = extract_word(input, &i);
//             add_token(&head, new_token(word, T_WORD));
//             free(word);
//         }
//     }
//     return head;
// }

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
