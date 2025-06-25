/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramroma <ramroma@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 13:31:21 by ramroma           #+#    #+#             */
/*   Updated: 2025/06/25 14:36:52 by ramroma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static bool is_metachar(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

static t_tokentype get_metatype(const char *s)
{
    if (!strncmp(s, "<<", 2))
     return T_HEREDOC;
    if (!strncmp(s, ">>", 2))
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
    if (!tok) return NULL;
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

static char *extract_quoted(const char *src, int *i, char quote)
{
    int start = ++(*i);
    while (src[*i] && src[*i] != quote)
        (*i)++;
    char *res = ft_substr(src, start, *i - start);
    if (src[*i] == quote)
        (*i)++;
    return res;
}

static char *extract_word(const char *src, int *i)
{
    int start = *i;
    while (src[*i] && !is_metachar(src[*i]) && src[*i] != ' ' && src[*i] != '\"' && src[*i] != '\'')
        (*i)++;
    return ft_substr(src, start, *i - start);
}

t_tokenizer *tokenize_input(const char *input)
{
    int i = 0;
    t_tokenizer *head = NULL;

    while (input[i])
    {
        while (input[i] == ' ')
            i++;

        if (!input[i])
            break;

        if (input[i] == '\"' || input[i] == '\'')
        {
            char quote = input[i];
            char *word = extract_quoted(input, &i, quote);
            add_token(&head, new_token(word, T_WORD));
            free(word);
        }
        else if (!ft_strncmp(&input[i], "<<", 2) || !ft_strncmp(&input[i], ">>", 2))
        {
            add_token(&head, new_token(ft_substr(input, i, 2), get_metatype(&input[i])));
            i += 2;
        }
        else if (is_metachar(input[i]))
        {
            add_token(&head, new_token(ft_substr(input, i, 1), get_metatype(&input[i])));
            i++;
        }
        else
        {
            char *word = extract_word(input, &i);
            add_token(&head, new_token(word, T_WORD));
            free(word);
        }
    }
    return head;
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

