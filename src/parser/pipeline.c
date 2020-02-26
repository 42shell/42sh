/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 04:37:16 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/19 17:21:29 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void		add_process_arg(t_process *process, t_token *arg)
{
	t_token		**new;
	int			size;
	int			i;

	if (!process->argv)
	{
		process->argv = (t_token **)ft_xmalloc(sizeof(t_token *) * 2);
		process->argv[0] = arg;
		return ;
	}
	i = 0;
	size = 0;
	while (process->argv[i++])
		size++;
	new = (t_token **)ft_xmalloc(sizeof(t_token *) * (size + 2));
	ft_memcpy((char *)new, (char *)process->argv, (size * sizeof(t_token *)));
	new[size] = arg;
	free(process->argv);
	process->argv = new;
}

static void		add_process_redir(t_process *process, t_redir *redir)
{
	t_redir		**new;
	int			size;
	int			i;

	if (!process->redirs)
	{
		process->redirs = (t_redir **)ft_xmalloc(sizeof(t_redir *) * 2);
		process->redirs[0] = redir;
		return ;
	}
	i = 0;
	size = 0;
	while (process->redirs[i++])
		size++;
	new = (t_redir **)ft_xmalloc(sizeof(t_redir *) * (size + 2));
	ft_memcpy((char *)new, (char *)process->redirs, (size * sizeof(t_token *)));
	new[size] = redir;
	free(process->redirs);
	process->redirs = new;
}

/*
** returns commands in this format:
**
** ls -l 2 > test -a -f
**
** t_node
** {
** 		data =		t_process
** 					{
** 						t_token **argv		= { ls, -l, -a, -f };
** 						t_redir **redirs	= { 2> };
** 					}
** 		childs = 	NULL;
** }
*/

static t_node	*command(void)
{
	t_node		*command_node;
	t_process	*process;
	t_redir		*redirect;

	process = process_new();
	command_node = node_new(process);
	while (g_parser.token
	&& ((redirect = io_redirect()) || g_parser.token->type == WORD))
	{
		if (redirect)
			add_process_redir(process, redirect);
		else if (g_parser.token->type == WORD)
		{
			add_process_arg(process, g_parser.token);
			g_parser.token = get_next_token();
		}
	}
	if (!process->argv && !process->redirs)
		ft_memdel((void **)&command_node);
	return (command_node);
}

/*
** pipeline			: command PIPE pipeline
** 					| command
**
** returns pipes in this format :
** ex:				ls | cat | wc:
**
**						"|"
**                     /   \
**                    ls   "|"
**                        /   \
**                      cat    wc
**
** (commands are actually stored with a NULL data node whose children contain
** the words, see above)
*/

t_node			*pipeline(void)
{
	t_node	*command_node;
	t_node	*pipe_node;
	t_node	*next;

	pipe_node = NULL;
	if (!(command_node = command()))
	{
		g_parser.error = NO_CMD_BEFORE_PIPE;
		g_parser.error_near = ft_strdup(g_parser.token->value->str);
		token_del(&g_parser.token);
		return (NULL);
	}
	else if (g_parser.token && g_parser.token->type == PIPE)
	{
		pipe_node = node_new(g_parser.token);
		node_add_child(pipe_node, command_node);
		while (!(g_parser.token = get_next_token()))
			g_lexer.line_cont = 1;
		if (!(next = pipeline()))
			return (NULL);
		node_add_child(pipe_node, next);
	}
	return (pipe_node ? pipe_node : command_node);
}
