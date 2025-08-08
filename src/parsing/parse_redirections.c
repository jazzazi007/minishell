#include "../../include/minishell.h"

int ft_is_redir(char *s)
{
	if (!ft_strncmp(s, "<<", 2))
		return 1;
	if (!ft_strncmp(s, ">>", 2))
		return 1;
	if (*s == '<')
		return 1;
	if (*s == '>')
		return 1;
	return 0;
}

int ft_fill_redir(t_redir **r_redir, t_tokenizer *curr, t_minishell *shell)
{
	int status;
	t_redir  *c_red;
	
	status = 1;
	c_red = ft_add_redir(r_redir);
	if (!c_red)
		return (-1);
	if (curr->type == T_REDIR_OUT)
		c_red->red_type = 0;
	else if (curr->type == T_APPEND)
		c_red->red_type = 1;
	else if (curr->type == T_REDIR_IN)
		c_red->red_type = 2;
	else if (curr->type == T_HEREDOC)
	{
		c_red->red_type = 3;
		if (curr->next && curr->next->type == T_FILE)
			c_red->here_fd = ft_open_heredoc(curr->next->value, shell);
	}
	else
		status = 0;
	if (status == 1 && curr->next)
	{
		c_red->filename = ft_strdup(curr->next->value);
		if (!c_red->filename)
			return (-1);		
	}
	return (status);
}
