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

static t_node	*pipe_list(t_node *left_command)
{
	t_node	*left_pipe;
	t_node	*right_command;
	t_node	*right_pipe;

	left_pipe = NULL;
	if (g_token && g_token->type == PIPE)
	{
		left_pipe = node_new(g_token);
		node_add_child(left_pipe, left_command);
		if (!(g_token = get_next_token())
		|| !(right_command = command()))
			return (parse_error(NO_CMD_AFTER_PIPE, "|", NULL));
		else if ((right_pipe = pipe_list(right_command)))
			node_add_child(left_pipe, right_pipe);
		else
			node_add_child(left_pipe, right_command);
	}
	return (left_pipe);
}

/*
**	pipeline         : command pipe_list
**	returns pipes in this format :
**	ex:           ls | cat | wc:
**
**
**						"|"
**                     /   \
**                    ls   "|"
**                        /   \
**                      cat    wc
**	(commands are actually stored with a NULL data node whose children contain
**	the words, see above)
*/

t_node			*pipeline(void)
{
	t_node *left_command;
	t_node *pipe;

	if (!(left_command = command()))
		return (parse_error(NO_CMD_BEFORE_PIPE, g_token->value->str, NULL));
	else if ((pipe = pipe_list(left_command)))
		return (pipe);
	return (left_command);
}
