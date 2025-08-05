#include "../include/minishell.h"

void ft_print_cmd(t_cmd **r_cmds)
{
	t_cmd *curr;
	int i;
	t_redir *redir;

	if (!r_cmds || !*r_cmds)
		return ;
	curr = *r_cmds;
	while(curr != NULL)
	{
		i = 0;
		redir = curr->redir;
		while (curr->args[i] != NULL)
		{
			printf("arg[%d]: %s\n", i, curr->args[i]);
			i++;
		}
		while (redir)
		{
			printf("redir: type %d, filename: %s, heredoc: %d\n", redir->red_type,
				redir->filename, redir->here_fd);
			redir = redir->next;
		}
		curr = curr->next;
		printf("------------------\n");
	}
}

void ft_print_tokens(t_tokenizer **r_tokens)
{
    t_tokenizer *curr;

    if (!r_tokens || !*r_tokens)
        return ;
    curr = *r_tokens;
    while (curr != NULL)
    {
        printf("Token: %-10s | Type: %d\n", curr->value, curr->type);
        curr = curr->next;
    }
}
