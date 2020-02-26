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
	i = -1;
	while (++i < size)
		new[i] = process->argv[i];
	new[i] = arg;
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
	i = -1;
	while (++i < size)
		new[i] = process->redirs[i];
	new[i] = redir;
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
	while (g_token
	&& ((redirect = io_redirect()) || g_token->type == WORD))
	{
		if (redirect)
			add_process_redir(process, redirect);
		else if (g_token->type == WORD)
		{
			add_process_arg(process, g_token);
			g_token = get_next_token();
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

t_node			*pipeline()
{
	t_node	*command_node;
	t_node	*pipe_node;
	t_node	*next;

	pipe_node = NULL;
	if (!(command_node = command()))
		return ((t_node *)parse_error(NO_CMD_BEFORE_PIPE,
									g_token->value->str, NULL));
	else if (g_token && g_token->type == PIPE)
	{
		pipe_node = node_new(g_token);
		node_add_child(pipe_node, command_node);
		while (!(g_token = get_next_token()))
			g_lexer.line_cont = 1;
		if (!(next = pipeline()))
			return (NULL);
		node_add_child(pipe_node, next);
	}
	return (pipe_node ? pipe_node : command_node);
}
