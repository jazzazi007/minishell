/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builed_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:05:14 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/08/04 14:49:36 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
	}
}

int ft_count_pipe(t_tokenizer **r_tokens)
{
	int count;
	t_tokenizer *curr;

	count = 0;
	curr = *r_tokens;
	while(curr->next != NULL)
	{
		if (ft_strncmp(curr->value, "|", 1) == 0)
			count++;
		curr = curr->next;
	}
	return (count);
}

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
	while(c_red->next != NULL)
		c_red = c_red->next;
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
		{
			//free all args
			return (-1);
		}
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
	// int				pipe_count;
	t_cmd			*cmd;
	t_cmd			*c_cmd;
	int 			size;
	int				i;
	
	curr = *r_tokens;
	// pipe_count = ft_count_pipe(r_tokens);
	cmd = NULL;
	size = ft_find_size(r_tokens);
	printf("size: %d\n", size);
	while (curr != NULL)
	{
		c_cmd = ft_add_cmd(&cmd); //traverse
		i = 0;
		c_cmd->args = malloc(sizeof(char *) * (size + 1));
		if (!c_cmd->args)
		{
			free(cmd);//change it to free all cmds
			printf("error\n");
			return (NULL);
		}
		while (curr && ft_strncmp(curr->value, "|", 1) != 0)
		{
			if (ft_is_redir(curr->value))
				ft_fill_redir(&c_cmd->redir, curr);
			else
				if (ft_fill_args(&c_cmd, curr, &i) < 0)
				{
					//free all cmds
					printf("error filling args\n");
					return (NULL);
				}
			curr = curr->next;
		}
	}
	printf("here\n");
	return (cmd);
}
