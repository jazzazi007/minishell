/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_add.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felayan <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 21:09:34 by felayan           #+#    #+#             */
/*   Updated: 2025/09/21 21:12:00 by felayan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_single_quotes(t_shell *dt, const char *input, int *index)
{
	int		sub_len;
	int		start;
	char	*sub;

	(*index)++;
	sub = NULL;
	start = *index;
	sub_len = 0;
	while (input[*index] && input[*index] != '\'')
	{
		(*index)++;
		sub_len++;
	}
	sub = ft_substr(input, start, sub_len);
	if (!sub)
		clean_shell(dt, MALLOC_FAILURE);
	add_token(dt, T_SINGLE, sub, false);
	if (input[*index] == '\'')
		(*index)++;
}

void	add_double_quotes(t_shell *dt, const char *input, int *index)
{
	int		sub_len;
	int		start;
	char	*sub;
	bool	is_expandable;

	(*index)++;
	sub = NULL;
	start = *index;
	sub_len = 0;
	is_expandable = false;
	while (input[*index] && input[*index] != '\"')
	{
		if (input[*index] == '$' && is_var(input[*index + 1]))
			is_expandable = true;
		(*index)++;
		sub_len++;
	}
	sub = ft_substr(input, start, sub_len);
	if (!sub)
		clean_shell(dt, MALLOC_FAILURE);
	add_token(dt, T_DOUBLE, sub, is_expandable);
	if (input[*index] == '\"')
		(*index)++;
}

void	add_operator(t_shell *dt, const char *input, int *index)
{
	char	*sub;
	t_tk	oper_t;

	oper_t = get_opertype(&input[*index]);
	sub = NULL;
	if (oper_t == T_REDIR_IN || oper_t == T_REDIR_OUT || oper_t == T_PIPE)
	{
		sub = ft_substr(input, *index, 1);
		(*index)++;
	}
	else if (oper_t == T_APPEND || oper_t == T_HEREDOC)
	{
		sub = ft_substr(input, *index, 2);
		*index += 2;
	}
	if (!sub)
		clean_shell(dt, MALLOC_FAILURE);
	add_token(dt, oper_t, sub, false);
}

void	add_word(t_shell *dt, const char *input, int *index)
{
	int		sub_len;
	int		start;
	char	*sub;
	bool	is_expandable;

	sub = NULL;
	start = *index;
	sub_len = 0;
	is_expandable = false;
	while (input[*index] && is_word(&input[*index]))
	{
		if (input[*index] == '$' && is_var(input[*index + 1]))
			is_expandable = true;
		(*index)++;
		sub_len++;
	}
	sub = ft_substr(input, start, sub_len);
	if (!sub)
		clean_shell(dt, MALLOC_FAILURE);
	add_token(dt, T_WORD, sub, is_expandable);
}
