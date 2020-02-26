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
*/

static t_node	*command(void)
{
	t_node		*command;
	t_node		*redirect;
	t_node		*word;

	command = node_new(NULL);
	while (g_token
	&& ((redirect = io_redirect()) || g_token->type == WORD))
	{
		if (redirect)
			node_add_child(command, redirect);
		else if (g_token->type == WORD)
		{
			word = node_new(g_token);
			node_add_child(command, word);
			g_token = get_next_token();
		}
	}
	if (command->nb_children == 0)
		ft_memdel((void *)&command);
	return (command);
}

/*
**  pipeline         : command PIPE pipeline
**					 | command
**
**	returns pipes in this format :
**	ex:           ls | cat | wc:
**
**						"|"
**                     /   \
**                    ls   "|"
**                        /   \
**                      cat    wc
** 
**	(commands are actually stored with a NULL data node whose children contain
**	the words, see above)
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
