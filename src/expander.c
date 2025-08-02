/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 18:14:56 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/08/02 20:05:13 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	update_quotes(char c, int *squote, int *dquote)
{
	if (c == '\'' && !(*dquote))
		*squote ^= 1;
	else if (c == '"' && !(*squote))
		*dquote ^= 1;
}

static char	*append_expanded_part(char *res, t_expand_ctx *ctx)
{
	char	*tmp;

	if (ctx->input[*(ctx->i)] == '$' && !ctx->in_single)
	{
		// write(1, "here\n", 5);
		(*(ctx->i))++;
		tmp = expand_var(ctx->input, ctx->i, ctx->sh);
		res = ft_strappend(res, tmp);
		printf("res:: %s\n", res);
		free(tmp);
	}
	else
	{
		tmp = ft_substr(ctx->input, *(ctx->i), 1);
		res = ft_strappend(res, tmp);
		free(tmp);
		(*(ctx->i))++;
	}
	return (res);
}

char	*expand_token_value(char *input, t_minishell *sh)
{
	char			*res;
	int				i;
	t_expand_ctx	ctx;

	res = ft_strdup("");
	i = 0;
	ctx.input = input;
	ctx.i = &i;
	ctx.sh = sh;
	ctx.in_single = 0;
	ctx.in_double = 0;
	// ft_putnbr_fd(sh->last_exit, 2);
	write(1, "\n", 1);
	while (input[i])
	{
		update_quotes(input[i], &ctx.in_single, &ctx.in_double);
		if ((input[i] == '\'' && !ctx.in_double)
			|| (input[i] == '"' && !ctx.in_single))
			i++;
		else
			res = append_expanded_part(res, &ctx);
	}
	return (res);
}

void	expand_tokens(t_tokenizer *head, t_minishell *shell)
{
	char	*expanded;

	while (head)
	{
		if (head->type == T_WORD)
		{
			if (head->in_single_quotes)
				expanded = ft_strdup(head->value);
			else
				expanded = expand_token_value(head->value, shell);
			free(head->value);
			head->value = expanded;
		}
		head = head->next;
	}
}
