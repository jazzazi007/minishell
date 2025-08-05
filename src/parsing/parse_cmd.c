#include "../../include/minishell.h"

int ft_parse_cmd(t_minishell **r_shell, const char *input)
{
    t_tokenizer *tokens;

    tokens = tokenize_input(input);
	if (!tokens || is_syntax_error(tokens))
	{
		free_tokens(tokens);
		return (-1);
	}
    ft_print_tokens(&tokens);
	expand_tokens(tokens, *r_shell);
	(*r_shell)->cmds = build_cmd(&tokens);
	free_tokens(tokens);
	if (!(*r_shell)->cmds || (*r_shell)->cmds == NULL)
	{
		(*r_shell)->exit_status = 1;
		return (-1);
	}
	ft_print_cmd(&(*r_shell)->cmds);
    return (0);
}
