#include "minishell.h"

void	add_single_quotes(t_shell *dt, const char *input, int *i)
{
	int		sub_len;
	int		start;
	char	*sub;

	(*i)++;
	sub = NULL;
	start = *i;
	sub_len = 0;
	while (input[*i] && input[*i] != '\'')
	{
		(*i)++;
		sub_len++;
	}
	sub = ft_substr(input, start, sub_len);
	if (!sub)
		clean_shell(dt, MALLOC_FAILURE);
	add_token(dt, T_SINGLE, sub, false);
	if (input[*i] == '\'')
		(*i)++;
}

void	add_double_quotes(t_shell *dt, const char *input, int *i)
{
	int		sub_len;
	int		start;
	char	*sub;
	bool	is_expandable;

	(*i)++;
	sub = NULL;
	start = *i;
	sub_len = 0;
	is_expandable = false;
	while (input[*i] && input[*i] != '\"')
	{
		if (input[*i] == '$' && is_var(input[*i + 1]))
			is_expandable = true;
		(*i)++;
		sub_len++;
	}
	sub = ft_substr(input, start, sub_len);
	if (!sub)
		clean_shell(dt, MALLOC_FAILURE);
	add_token(dt, T_DOUBLE, sub, is_expandable);
	if (input[*i] == '\"')
		(*i)++;
}

void	add_operator(t_shell *dt, const char *input, int *i)
{
	char	*sub;
	t_tk	meta_t;

	meta_t = get_opertype(&input[*i]);
	sub = NULL;
	if (meta_t == T_REDIR_IN || meta_t == T_REDIR_OUT || meta_t == T_PIPE)
	{
		sub = ft_substr(input, *i, 1);
		(*i)++;
	}
	else if (meta_t == T_APPEND || meta_t == T_HEREDOC)
	{
		sub = ft_substr(input, *i, 2);
		*i += 2;
	}
	if (!sub)
		clean_shell(dt, MALLOC_FAILURE);
	add_token(dt, meta_t, sub, false);
}

void	add_word(t_shell *dt, const char *input, int *i)
{
	int		sub_len;
	int		start;
	char	*sub;
	bool	is_expandable;

	sub = NULL;
	start = *i;
	sub_len = 0;
	is_expandable = false;
	while (input[*i] && is_word(&input[*i]))
	{
		if (input[*i] == '$' && is_var(input[*i + 1]))
			is_expandable = true;
		(*i)++;
		sub_len++;
	}
	sub = ft_substr(input, start, sub_len);
	if (!sub)
		clean_shell(dt, MALLOC_FAILURE);
	add_token(dt, T_WORD, sub, is_expandable);
}
