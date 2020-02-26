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

/*
**	returns commands in this format:
**
**	ls -l 2 > test -a -f
**
**                                 NULL
**                          /   /   |   \   \
**                         ls  -l   >   -a  -f
**                                 / \
**                                2   test
**
** 			t_process     { char **argv; t_node *redirs (array or list) }
**

*/

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
	else
	{
		i = 0;
		size = 0;
		while (process->argv[i++])
			size++;
		new = (t_token **)ft_xmalloc(sizeof(t_token *) * (size + 2));
		i = 0;
		while (i < size)
		{
			new[i] = process->argv[i];
			i++;
		}
		new[i] = arg;
	}
	free(process->argv);
	process->argv = new;
}

static void		add_process_redir(t_process *process, t_node *redir)
{
	t_node		**new;
	int			size;
	int			i;

	if (!process->redirs)
	{
		process->redirs = (t_node **)ft_xmalloc(sizeof(t_node *) * 2);
		process->redirs[0] = redir;
		return ;
	}
	else
	{
		i = 0;
		size = 0;
		while (process->redirs[i++])
			size++;
		new = (t_node **)ft_xmalloc(sizeof(t_node *) * (size + 2));
		i = 0;
		while (i < size)
		{
			new[i] = process->redirs[i];
			i++;
		}
		new[i] = redir;
	}
	free(process->redirs);
	process->redirs = new;
}


static t_node	*command(void)
{
	t_node		*command_node;
	t_process	*process;
	t_node		*redirect;

	process = process_new();
	command_node = node_new(process, PROCESS);
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
		return (parse_error(NO_CMD_BEFORE_PIPE, g_token->value->str, NULL));
	else if (g_token && g_token->type == PIPE)
	{
		pipe_node = node_new(g_token, 0);
		node_add_child(pipe_node, command_node);
		while (!(g_token = get_next_token()))
			g_lexer.line_cont = 1;
		if (!(next = pipeline()))
			return (NULL);
		node_add_child(pipe_node, next);
	}
	return (pipe_node ? pipe_node : command_node);
}
