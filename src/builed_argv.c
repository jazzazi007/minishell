/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builed_argv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:05:14 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/07/17 19:05:51 by ralbliwi         ###   ########.fr       */
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
    int argc = count_tokens(tokens);
    char **argv = malloc(sizeof(char *) * (argc + 1));
    if (!argv)
        return NULL;

    int i = 0;
    while (tokens)
    {
        argv[i] = ft_strdup(tokens->value);  // Copy cleaned token value
        tokens = tokens->next;
        i++;
    }
    argv[i] = NULL;
    return argv;
}
