#include "minishell.h"

t_tk get_opertype(const char *s)
{
	if (!ft_strncmp(s, "<<", 2))
		return T_HEREDOC;
	if (!ft_strncmp(s, ">>", 2))
		return T_APPEND;
	if (*s == '<')
		return T_REDIR_IN;
	if (*s == '>')
		return T_REDIR_OUT;
	if (*s == '|')
		return T_PIPE;
	return T_WORD;
}

static t_tokenizer	*get_last_token(t_tokenizer *tokens)
{
	if (!tokens)
		return (NULL);
	while (tokens -> next)
		tokens = tokens -> next;
	return (tokens);
}

void	add_token(t_shell *dt, t_tk t_type, char *token, bool exp)
{
	t_tokenizer	*new;
	t_tokenizer	*last;

	last = NULL;
	new = malloc(sizeof(t_tokenizer));
	if (!new)
		clean_shell(dt, MALLOC_FAILURE);
	new -> value = token;
	new -> type = t_type;
	new -> is_expandable = exp;
	new -> next = NULL;
	if (!dt -> tokens)
		dt -> tokens = new;
	else
	{
		last = get_last_token(dt -> tokens);
		last -> next = new;
	}
}
