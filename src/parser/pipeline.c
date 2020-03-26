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
** for the moment,
** pipeline	= pipe_sequence
**
** pipeline			: command PIPE linebreak pipeline
**					: command
**
**                 |
**               /   \
**             |     test
**           /   \
**         ls    cat
*/

t_node			*ps_pipe_sequence(void)
{
	t_node		*pipe_seq;
	t_node		*node;

	pipe_seq = ps_command();
	while (pipe_seq && g_parser.token->type == PIPE)
	{
		node = (t_node *)ft_xmalloc(sizeof(t_node));
		node->type = NODE_PIPE;
		node->left = pipe_seq;
		token_del(&g_parser.token);
		g_parser.token = get_next_token();
		ps_linebreak(PIPE);
		if (!(node->right = ps_command()))
		{
			g_parser.error = g_parser.error ? g_parser.error : NO_CMD_AFTER_OP;
			ast_del(&node);
		}
		pipe_seq = node;
	}
	return (pipe_seq);
}

t_node			*ps_pipeline(void)
{
	t_node		*pipeline;

	if (!g_parser.token)
		return (NULL);
	if (g_parser.token->type == BANG)
	{
		pipeline = (t_node *)ft_xmalloc(sizeof(t_node));
		pipeline->type = NODE_BANG;
		g_parser.token = get_next_token();
		token_del(&g_parser.token);
		if (!(pipeline->left = ps_pipe_sequence()))
			ast_del(&pipeline);
	}
	else
		pipeline = ps_pipe_sequence();
	return (pipeline);
}