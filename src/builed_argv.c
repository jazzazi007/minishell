/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builed_argv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramroma <ramroma@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:05:14 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/07/18 10:42:28 by ramroma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int count_tokens(t_tokenizer *tokens)
{
    int count = 0;
    while (tokens)
    {
        count++;
        tokens = tokens->next;
    }
    return count;
}

// Build argv array copying tokens' values
char **build_argv(t_tokenizer *tokens)
{
     int count = 0;
    t_tokenizer *tmp = tokens;
    while (tmp)
    {
        count++;
        tmp = tmp->next;
    }

    char **argv = malloc(sizeof(char *) * (count + 1));
    if (!argv)
        return NULL;

    tmp = tokens;
    for (int i = 0; i < count; i++)
    {
        argv[i] = strip_quotes(tmp->value);
        tmp = tmp->next;
    }
    argv[count] = NULL;
    return argv;
}
