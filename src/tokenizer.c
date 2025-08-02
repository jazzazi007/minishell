/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 13:31:21 by ramroma           #+#    #+#             */
/*   Updated: 2025/08/02 18:54:18 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


// static void	update_quotes(char c, int *squote, int *dquote)
// {
// 	if (c == '\'' && !(*dquote))
// 		*squote ^= 1;
// 	else if (c == '"' && !(*squote))
// 		*dquote ^= 1;
// }

// static char	*extract_quoted_segment(const char *input, int *i)
// {
// 	char	quote;
// 	int		start;
// 	char	*part;

// 	quote = input[(*i)++];
// 	start = *i;
// 	while (input[*i] && input[*i] != quote)
// 		(*i)++;
// 	if (!input[*i])
// 		return (NULL);
// 	part = ft_substr(input, start, *i - start);
// 	(*i)++;
// 	return (part);
// }

// static char	*extract_unquoted_segment(const char *input, int *i)
// {
// 	int	start;

// 	start = *i;
// 	while (input[*i] && input[*i] != ' ' && !is_metachar(input[*i])
// 		&& input[*i] != '\'' && input[*i] != '\"')
// 		(*i)++;
// 	return (ft_substr(input, start, *i - start));
// }

// static char	*append_part(char *word, char *part)
// {
// 	char	*tmp;

// 	tmp = word;
// 	word = ft_strjoin(word, part);
// 	free(tmp);
// 	free(part);
// 	return (word);
// }

// static char	*extract_word_combined(const char *input, int *i,
// 	int *in_single, int *in_double)
// {
// 	char	*word;
// 	char	*part;

// 	*in_single = 0;
// 	*in_double = 0;
// 	word = ft_strdup("");
// 	while (input[*i] && input[*i] != ' ' && !is_metachar(input[*i]))
// 	{
// 		part = NULL;
// 		if (input[*i] == '\'' || input[*i] == '\"')
// 		{
// 			update_quotes(input[*i], in_single, in_double);
// 			part = extract_quoted_segment(input, i);
// 			if (!part)
// 			{
// 				free(word);
// 				return (NULL);
// 			}
// 			update_quotes(input[*i - 1], in_single, in_double);
// 		}
// 		else
// 			part = extract_unquoted_segment(input, i);
// 		word = append_part(word, part);
// 	}
// 	return (word);
// }

static bool	handle_metacharacters(const char *input, int *i, t_tokenizer **head)
{
	if (!ft_strncmp(&input[*i], "<<", 2)
		|| !ft_strncmp(&input[*i], ">>", 2))
	{
		add_token(head, new_token(
				ft_substr(input, *i, 2),
				get_metatype(&input[*i])));
		*i += 2;
		return (true);
	}
	else if (is_metachar(input[*i]))
	{
		add_token(head, new_token(
				ft_substr(input, *i, 1),
				get_metatype(&input[*i])));
		(*i)++;
		return (true);
	}
	return (false);
}

// static int	process_token(const char *input, int *i, t_tokenizer **head)
// {
// 	char		*word;
// 	int			in_single;
// 	int			in_double;
// 	t_tokenizer	*new;

// 	while (input[*i] == ' ')
// 		(*i)++;
// 	if (!input[*i])
// 		return (0);
// 	if (handle_metacharacters(input, i, head))
// 		return (1);
// 	word = extract_word_combined(input, i, &in_single, &in_double);
// 	if (!word)
// 	{
// 		free_tokens(*head);
// 		*head = NULL;
// 		return (-1);
// 	}
// 	new = new_token(word, T_WORD);
// 	new->in_single_quotes = in_single;
// 	new->in_double_quotes = in_double;
// 	add_token(head, new);
// 	free(word);
// 	return (1);
// }

// t_tokenizer	*tokenize_input(const char *input)
// {
// 	t_tokenizer	*head;
// 	int			i;
// 	int			ret;

// 	head = NULL;
// 	i = 0;
// 	while (input[i])
// 	{
// 		ret = process_token(input, &i, &head);
// 		if (ret <= 0)
// 			break ;
// 	}
// 	if (ret == -1)
// 		return (NULL);
// 	return (head);
// }

static char *extract_quoted_segment(const char *input, int *i)
{
    char quote;
    int start;
    char *part;

    quote = input[(*i)++];
    start = *i;
    while (input[*i] && input[*i] != quote)
        (*i)++;
    part = ft_substr(input, start, *i - start);
    if (input[*i] == quote)
        (*i)++;
    return (part);
}

static char *extract_unquoted_segment(const char *input, int *i)
{
    int start;

    start = *i;
    while (input[*i] && input[*i] != ' ' && !is_metachar(input[*i])
        && input[*i] != '\'' && input[*i] != '\"')
        (*i)++;
    return (ft_substr(input, start, *i - start));
}

static char *extract_token_value(const char *input, int *i)
{
    char *token;
    char *part;
    char *tmp;

    token = ft_strdup("");
    while (input[*i] && input[*i] != ' ' && !is_metachar(input[*i]))
    {
        if (input[*i] == '\'' || input[*i] == '"')
            part = extract_quoted_segment(input, i);
        else
            part = extract_unquoted_segment(input, i);
        tmp = token;
        token = ft_strjoin(token, part);
        free(tmp);
        free(part);
    }
    return (token);
}

t_tokenizer *tokenize_input(const char *input)
{
    t_tokenizer *head;
    t_tokenizer *new_token_node;
    char *token_value;
    int i;

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
        token_value = extract_token_value(input, &i);
        new_token_node = new_token(token_value, T_WORD);
        free(token_value);
        add_token(&head, new_token_node);
    }
    return (head);
}
