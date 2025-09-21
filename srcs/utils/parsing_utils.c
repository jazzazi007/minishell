/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builed_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralbliwi <ralbliwi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:05:14 by ralbliwi          #+#    #+#             */
/*   Updated: 2025/08/19 16:27:32 by ralbliwi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int ft_fill_args(t_cmd **r_cmd, t_tokenizer *token, int *i)
// {
// 	if (token->type == T_WORD || token ->type == T_DOUBLE || token -> type == T_SINGLE)
// 	{
// 		(*r_cmd)->args[*i] = ft_strdup(token->value);
// 		if (!(*r_cmd)->args[*i])
// 			return (-1);
// 		(*i)++;
// 	}
// 	(*r_cmd)->args[*i] = NULL;
// 	return (0);
// }

// int	ft_find_size(t_tokenizer **r_tokens)
// {
// 	int size;
// 	t_tokenizer *curr;

// 	size = 0;
// 	curr = *r_tokens;
// 	while(curr)
// 	{
// 		if (curr->type == T_WORD)
// 			size++;
// 		curr = curr->next;
// 	}
// 	return (size);
// }

// t_cmd *ft_create_cmd(t_cmd **r_cmd, t_tokenizer **r_tokens)
// {
// 	t_cmd *new;
// 	int 			size;

// 	size = ft_find_size(r_tokens);
// 	new = ft_add_cmd(r_cmd);
// 	new->redir = NULL;
// 	if (!new)
// 	{
// 		printf("error in creating cmd\n");
// 		return (NULL);
// 	}
// 	new->args = malloc(sizeof(char *) * (size + 1));
// 	new->args[0] = NULL;
// 	if (!new->args)
// 	{
// 		printf("error in allocating args\n");
// 		return (NULL);
// 	}
// 	return (new);
// }

// t_cmd *build_cmd(t_shell *shell)
// {
// 	t_tokenizer				*curr;
// 	t_cmd			*cmd;
// 	t_cmd			*c_cmd;
// 	int				i;
	
// 	curr = shell -> tokens;
// 	cmd = NULL;
// 	while (curr != NULL)
// 	{
// 		i = 0;
// 		c_cmd = ft_create_cmd(&cmd, &shell -> tokens);
// 		if (!c_cmd)
// 		{
// 			clean_cmds(cmd);
// 			return (NULL);
// 		}
// 		while (curr && ft_strncmp(curr->value, "|", 1) != 0)
// 		{
// 			if (ft_is_redir(curr->value))
// 			{
// 				if (ft_fill_redir(&c_cmd->redir, &curr, shell) < 0)
// 				{
// 					clean_cmds(cmd);
// 					return (NULL);
// 				}
// 			}
// 			else
// 			{
// 				if (ft_fill_args(&c_cmd, curr, &i) < 0)
// 				{
// 					clean_cmds(cmd);
// 					printf("error filling args\n");
// 					return (NULL);
// 				}
// 				curr = curr->next;
// 			}
// 		}
// 		if (curr && !ft_strncmp(curr -> value,"|", 1))
// 			curr = curr -> next;
// 	}
// 	return (cmd);
// }
static void	token_cmd_count(t_tokenizer *tokens, int *wrds, int *rdrs)
{
	(*wrds) = 0;
	(*rdrs) = 0;
	while (tokens && !is_pipe(tokens -> value))
	{
		if (is_redir(tokens -> value))
		{
			(*rdrs)++;
			tokens = tokens -> next;
		}
		else
			(*wrds)++;
		tokens = tokens -> next;
	}
}

t_cmd	*init_cmd(t_shell *dt, t_tokenizer *tokens)
{
	t_cmd	*cmd;

	(void)dt;
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		clean_shell(dt, MALLOC_FAILURE);
	ft_memset(cmd, 0, sizeof(t_cmd));
	token_cmd_count(tokens, &cmd -> word_count, &cmd -> redir_count);
	// cmd -> pipe_fd[0] = -1;
	// cmd -> pipe_fd[1] = -1;
	// cmd -> pid = -1;
	cmd -> args = malloc(sizeof(char *) * (cmd ->word_count + 1));
	if (cmd -> redir_count > 0)
		cmd -> redir = malloc(sizeof(t_redir) * (cmd -> redir_count));
	if (!cmd -> args || (!cmd -> redir && cmd ->redir_count > 0))
	{
		free(cmd -> args);
		free(cmd -> redir);
		free(cmd);
		clean_shell(dt, MALLOC_FAILURE);
	}
	int i = 0;
	while (i < cmd -> redir_count)
		cmd -> redir[i++].filename = NULL;
	cmd -> args[cmd -> word_count] = NULL;
	return (cmd);
}

static t_cmd	*get_last_cmd(t_cmd *cmd)
{
	while (cmd && cmd -> next)
		cmd = cmd -> next;
	return (cmd);
}

void	append_cmd(t_shell *dt, t_cmd *new)
{
	t_cmd	*last;

	if (!dt -> cmds)
		dt -> cmds = new;
	else
	{
		last = get_last_cmd(dt -> cmds);
		last -> next = new;
	}
	dt -> cmd_count++;
}

static int	add_redir_cmd(t_cmd *cmd, t_tokenizer **tokens, int	*rdr_i, t_shell *dt)
{
	if (!ft_strcmp((*tokens)-> value, ">"))
		cmd -> redir[*rdr_i].red_type = OUT;
	else if (!ft_strcmp((*tokens)-> value, "<"))
		cmd -> redir[*rdr_i].red_type = IN;
	else if (!ft_strcmp((*tokens)-> value, ">>"))
		cmd -> redir[*rdr_i].red_type = APPEND;
	else if (!ft_strcmp((*tokens)-> value, "<<"))
	{
		cmd -> redir[*rdr_i].red_type = HEREDOC;
		signal(SIGINT, SIG_IGN);
		setup_signal_handlers();
		cmd -> redir[*rdr_i].here_fd = ft_open_heredoc((*tokens)->next, dt);
		if (g_exit_status == 130)
		{
			dt -> exit_status = g_exit_status;
			g_exit_status = 0;
			return (130);
		}
	}
	(*tokens) = (*tokens)-> next;
	if (cmd -> redir[*rdr_i].red_type != HEREDOC)
	{	
		cmd -> redir[*rdr_i].filename = ft_strdup((*tokens)-> value);
		if (!cmd -> redir[*rdr_i].filename)
			return (MALLOC_FAILURE);
	}
	(*rdr_i)++;
	return (SUCCESS);
}

static int	add_word_cmd(t_cmd *cmd, const char *token, int *wrd_i)
{
	cmd -> args[*wrd_i] = ft_strdup(token);
	if (!cmd -> args[*wrd_i])
		return (MALLOC_FAILURE);
	(*wrd_i)++;
	return (SUCCESS);
}

static int	add_cmd(t_cmd *cmd, t_tokenizer **tokens, int *rdr_i, int *wrd_i, t_shell *dt)
{
	int	status = 0;
	while (*tokens && !is_pipe((*tokens)-> value))
	{
		if (is_redir((*tokens)-> value))
		{
			status = add_redir_cmd(cmd, tokens, rdr_i, dt);
			if (status == MALLOC_FAILURE)
				return (MALLOC_FAILURE);
			else if (status == 130)
				return (130);
		}
		else
		{
			if (add_word_cmd(cmd, (*tokens)-> value, wrd_i))
				return (MALLOC_FAILURE);
		}
		(*tokens) = (*tokens)-> next;
	}
	return (SUCCESS);
}

int	tokens_to_cmd(t_shell *dt, t_tokenizer *tokens)
{
	t_cmd	*new_cmd;
	int		wrd_index;
	int		rdr_index;
	int		status = 0;
	int i;
	while (tokens)
	{
		i = 0;
		wrd_index = 0;
		rdr_index = 0;
		new_cmd = init_cmd(dt, tokens);
		status = add_cmd(new_cmd, &tokens, &rdr_index, &wrd_index, dt);
		if (status == MALLOC_FAILURE) // malloc fail
		{
			while (new_cmd -> args[i])
				free(new_cmd -> args[i++]);
			i = 0;
			while (i < new_cmd -> redir_count)
			{
				if (new_cmd -> redir[i].filename)
					free(new_cmd -> redir[i].filename);
				i++;
			}
			free(new_cmd -> args);
			free(new_cmd -> redir);
			free(new_cmd);
			new_cmd = NULL;
			clean_shell(dt, MALLOC_FAILURE);
		}
		else if (status == 130)
		{
			while (new_cmd -> args[i])
				free(new_cmd -> args[i++]);
			i = 0;
			while (i < new_cmd -> redir_count)
			{
				if (new_cmd -> redir[i].filename)
					free(new_cmd -> redir[i].filename);
				i++;
			}
			free(new_cmd -> args);
			free(new_cmd -> redir);
			free(new_cmd);
			new_cmd = NULL;
			return (130);
		}
		if (new_cmd)
			append_cmd(dt, new_cmd);
		if (tokens && is_pipe(tokens -> value))
			tokens = tokens -> next;
	}
	return (SUCCESS);
}
