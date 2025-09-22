/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felayan <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 21:10:09 by felayan           #+#    #+#             */
/*   Updated: 2025/09/21 22:02:09 by felayan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_var_token(const char *var, int *loc)
{
	int	start;

	start = *loc;
	while (var[*loc] && (ft_isalnum(var[*loc]) || var[*loc] == '_'))
		(*loc)++;
	return (ft_substr(var, start, *loc - start));
}

static int	expand_var(t_shell *dt, const char *token, char **expanded)
{
	int	i;

	i = 0;
	while (token[i])
	{
		if (token[i] != '$' || !is_var(token[i + 1]))
		{
			*expanded = append_char(*expanded, token[i++]);
			if (!*expanded)
				return (MALLOC_FAILURE);
		}
		else if (token[++i] == '?')
		{
			if (crt_var(dt -> envp, expanded, ft_itoa(dt-> exit_status), true))
				return (MALLOC_FAILURE);
			i++;
		}
		else
		{
			if (crt_var(dt -> envp, expanded, get_var_token(token, &i), false))
				return (MALLOC_FAILURE);
		}
	}
	return (SUCCESS);
}

void	expander(t_shell *dt)
{
	t_tokens	*current;
	char		*expanded;

	expanded = NULL;
	current = dt -> tokens;
	while (current)
	{
		if (current -> type == T_HEREDOC)
			current->next->is_expandable = false;
		if (current -> is_expandable)
		{
			expanded = ft_strdup("");
			if (!expanded || expand_var(dt, current -> value, &expanded))
				clean_shell(dt, MALLOC_FAILURE);
			free(current -> value);
			current -> value = expanded;
		}
		current = current -> next;
	}
}
