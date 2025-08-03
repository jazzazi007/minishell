/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builed_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:05:14 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/08/03 17:42:37 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


void ft_print_cmd(t_cmd **r_cmds)
{
	t_cmd *curr;
	int i;

	curr = *r_cmds;
	while(curr)
	{
		i = 0;
		while (curr->args[i] != NULL)
		{
			printf("arg[%d]: %s\n", i, curr->args[i]);
			i++;
		}
		// printf("")
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

int ft_fill_redir(t_cmd **r_cmd, t_tokenizer *curr)
{
	int status;
	
	printf("redir\n");
	status = 1;//change it to 2
	if (curr->type == T_REDIR_OUT)
		(*r_cmd)->red_type = 0;
	else if (curr->type == T_APPEND)
		(*r_cmd)->red_type = 1;
	else if (curr->type == T_REDIR_IN)
		(*r_cmd)->red_type = 2;
	else if (curr->type == T_HEREDOC)
	{
		//open heredoc here
		(*r_cmd)->red_type = 3;
	}
	else
	{
		printf("no redir\n");
		status = 0;
	}
	if (status == 1)
	{
		if (curr->next)
			(*r_cmd)->filename = curr->next->value;
	}
	return (status);
}

int ft_fill_args(t_cmd **r_cmd, t_tokenizer **r_tokens, int size,
	int *count)
{
	int i;
	t_tokenizer *curr;
	
	(*r_cmd)->args = malloc(sizeof(char *) * (size + 1));
	if (!(*r_cmd)->args)
		return (-1);
	curr = *r_tokens;
	i = 0;
	while(curr)
	{
		if (curr->type == T_WORD)
		{
			(*r_cmd)->args[i] = curr->value;
			i++;
		}
		curr = curr->next;
	}
	(*r_cmd)->args[i] = NULL;
	count += i;
	printf("count: %d\n", *count);
	return (i);
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
	int				pipe_count;
	t_cmd			*cmd;
	int 			size;
	int				i;
	
	curr = *r_tokens;
	pipe_count = ft_count_pipe(r_tokens);
	cmd = NULL;
	size = ft_find_size(r_tokens);
	printf("size of cmd:  %i\n", size);
	while (curr != NULL)
	{
		ft_add_cmd(&cmd);
		i = 0;
		while (curr && ft_strncmp(curr->value, "|", 1) != 0)
		{
			printf("i: %d\n", i);
			i += ft_fill_redir(&cmd, curr);
			printf("i: %d\n", i);
			if (ft_fill_args(&cmd, r_tokens, size, &i) < 0)
			{
				//free cmd;
				return (NULL);
			}
			curr = curr->next;
		}
		// if (curr->next)
		// 	curr = curr->next;
		printf("HI\n");
		
	}
	return (cmd);
}
