/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builed_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:05:14 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/08/06 14:41:09 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

t_cmd *ft_create_cmd(t_cmd **r_cmd, t_tokenizer **r_tokens)
{
	t_cmd *new;
	int 			size;

	size = ft_find_size(r_tokens);
	new = ft_add_cmd(r_cmd);
	if (!new)
	{
		printf("error in creating cmd\n");
		return (NULL);
	}
	new->args = malloc(sizeof(char *) * (size + 1));
	if (!new->args)
	{
		printf("error in allocating args\n");
		return (NULL);
	}
	return (new);
}

t_cmd *build_cmd(t_tokenizer **r_tokens, t_minishell *shell)
{
	t_tokenizer				*curr;
	t_cmd			*cmd;
	t_cmd			*c_cmd;
	int				i;
	
	if (!r_tokens || !*r_tokens)
		return (NULL);
	curr = *r_tokens;
	cmd = NULL;
	while (curr != NULL)
	{
		i = 0;
		printf("Building command...\n");
		c_cmd = ft_create_cmd(&cmd, r_tokens);
		if (!c_cmd)
		{
			ft_free_cmds(cmd);
			return (NULL);
		}
		while (curr && ft_strncmp(curr->value, "|", 1) != 0)
		{
			printf("Processing token: %s\n", curr->value);
			if (ft_is_redir(curr->value))
			{
				if (ft_fill_redir(&c_cmd->redir, curr, shell) < 0)
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
		if (curr)
			curr = curr->next; //skip the pipe token
	}
	return (cmd);
}
