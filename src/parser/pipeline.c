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

static void	linebreak_get_input(void)
{
	linebreak();
	while (!g_parser.error && !g_parser.token)
	{
		g_lexer.line_cont = PIPE;
		g_parser.token = get_next_token();
	}
}

t_node			*pipeline(void)
{
	t_node	*left_command;
	t_node	*right_pipeline;
	t_node	*pipe_node;

	pipe_node = NULL;
	if (g_parser.error || !(left_command = command()))
	{
		parse_error(NO_CMD_BEFORE_PIPE,
		g_parser.token ? ft_strdup(g_parser.token->value->str) : NULL);
		token_del(&g_parser.token);
		return (NULL);
	}
	else if (g_parser.token && g_parser.token->type == PIPE)
	{
		pipe_node = node_new(g_parser.token, NODE_PIPE);
		node_add_child(pipe_node, left_command);
		g_parser.token = get_next_token();
		linebreak_get_input();
		if (!(right_pipeline = pipeline()))
			return (NULL);
		node_add_child(pipe_node, right_pipeline);
	}
	return (pipe_node ? pipe_node : left_command);
}
