/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:37:40 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/08/19 17:58:33 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
    	signal(SIGINT, SIG_IGN);
		c_red->here_fd = ft_open_heredoc(curr->next->value, shell);
		if (g_exit_status == 130)
		{
			shell->exit_status = 130;
			g_exit_status = 0;
			return -1;
		}
		setup_signal_handlers();
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
