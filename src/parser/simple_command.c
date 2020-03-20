/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/13 18:32:06 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** lol I forgot about expansion
*/

static void		add_process_word(t_process *process, t_token *word)
{
	t_token		*ptr;

	if (!process->words)
		process->words = word;
	else
	{
		ptr = process->words;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = word;
	}
	/*
	char		**new;
	int			size;

	if (!process->argv)
	{
		process->argv = (char **)ft_xmalloc(sizeof(char *) * 2);
		process->argv[0] = ft_strdup(arg->value->str);
		return ;
	}
	size = 0;
	while (process->argv[size])
		size++;
	new = (char **)ft_xmalloc(sizeof(char *) * (size + 2));
	ft_memcpy((char *)new, (char *)process->argv, (size * sizeof(char *)));
	new[size] = ft_strdup(arg->value->str);
	free(process->argv);
	process->argv = new;
	token_del(&arg);
	*/
}

static void		add_process_redir(t_process *process, t_redir *redir)
{
	t_redir		*ptr;

	if (!process->redirs)
		process->redirs = redir;
	else
	{
		ptr = process->redirs;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = redir;
	}
}

static int		is_valid_argv(t_token *token)
{
	return (token
	&& (token->type == WORD 
	|| token->type == IO_NUMBER
	|| is_redir(token)));
}

/*
** simple_command	: WORD
** 					| WORD simple_command
**					| io_redir
**					| io_redir simple_command
**
** returns a t_process containing argv array and a list of redirections
*/

t_process		*ps_simple_command(void)
{
	t_process	*process;
	t_redir		*redir;

	if (g_parser.error || !is_valid_argv(g_parser.token))
	{
		g_parser.error = g_parser.error ? g_parser.error : NO_CMD_BEFORE_PIPE;
		return (NULL);
	}
	process = (t_process *)ft_xmalloc(sizeof(t_process));
	while (is_valid_argv(g_parser.token))
	{
		if (g_parser.token->type == WORD)
		{
			add_process_word(process, g_parser.token);
			g_parser.token = get_next_token();
		}
		else if (!(redir = ps_io_redirect()))
		{
			process_del(&process);
			return (NULL);
		}
		else
			add_process_redir(process, redir);
	}
	return (process);
}
