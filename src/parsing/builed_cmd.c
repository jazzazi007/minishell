/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builed_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:05:14 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/08/05 17:59:05 by codespace        ###   ########.fr       */
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

int ft_fill_redir(t_redir **r_redir, t_tokenizer *curr)
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
		//open heredoc here
		c_red->red_type = 3;
	}
	else
		status = 0;
	if (status == 1 && curr->next)
	{
		curr->next->type = T_FILE;
		c_red->filename = ft_strdup(curr->next->value);
		if (!c_red->filename)
			return (-1);		
	}
	return (status);
}

int ft_fill_args(t_cmd **r_cmd, t_tokenizer *token, int *i)
{
	if (token->type == T_WORD)
	{
		(*r_cmd)->args[*i] = ft_strdup(token->value);
		if (!(*r_cmd)->args[*i])
			return (-1);
		(*i)++;
	}
	(*r_cmd)->args[*i] = NULL;
	return (0);
}

int	ft_find_size(t_tokenizer **r_tokens)
{
	int size;
	t_tokenizer *curr;

	size = 0;
	curr = *r_tokens;
	while(curr)
	{
		if (curr->type == T_WORD)
			size++;
		curr = curr->next;
	}
	return (size);
}


// Build argv array copying tokens' values
t_cmd *build_cmd(t_tokenizer **r_tokens)
{
	t_tokenizer				*curr;
	t_cmd			*cmd;
	t_cmd			*c_cmd;
	int 			size;
	int				i;
	
	curr = *r_tokens;
	cmd = NULL;
	size = ft_find_size(r_tokens);
	while (curr != NULL)
	{
		c_cmd = ft_add_cmd(&cmd);
		i = 0;
		c_cmd->args = malloc(sizeof(char *) * (size + 1));
		if (!c_cmd->args)
		{
			ft_free_cmds(cmd);
			printf("error\n");
			return (NULL);
		}
		while (curr && ft_strncmp(curr->value, "|", 1) != 0)
		{
			if (ft_is_redir(curr->value))
			{
				if (ft_fill_redir(&c_cmd->redir, curr) < 0)
				{
					ft_free_cmds(cmd);
					printf("error filling redir\n");
					return (NULL);
				}
			}
			else
				if (ft_fill_args(&c_cmd, curr, &i) < 0)
				{
					ft_free_cmds(cmd);
					printf("error filling args\n");
					return (NULL);
				}
			curr = curr->next;
		}
		if (curr && curr->next)
			curr = curr->next; //skip the pipe token
	}
	return (cmd);
}
