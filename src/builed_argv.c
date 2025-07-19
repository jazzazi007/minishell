/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builed_argv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:05:14 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/07/19 12:45:24 by ralbliwi         ###   ########.fr       */
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
char	**build_argv(t_tokenizer *tokens)
{
	int				count;
	int				i;
	t_tokenizer		*tmp;
	char			**argv;

	count = count_tokens(tokens);
	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	tmp = tokens;
	i = 0;
	while (i < count)
	{
		argv[i] = strip_quotes(tmp->value);
		tmp = tmp->next;
		i++;
	}
	argv[count] = NULL;
	return (argv);
}
