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

t_node			*ps_pipeline(void)
{
	t_node		*pipeline;
	t_node		*node;

	pipeline = ps_command();
	while (pipeline && g_parser.token->type == PIPE)
	{
		node = (t_node *)ft_xmalloc(sizeof(t_node));
		node->type = PIPE;
		node->left = pipeline;
		token_del(&g_parser.token);
		g_parser.token = get_next_token();
		ps_linebreak(PIPE);
		if (!(node->right = ps_pipeline()))
		{
			g_parser.error = g_parser.error ? g_parser.error : NO_CMD_AFTER_OP;
			ast_del(&node);
		}
		pipeline = node;
	}
	return (pipeline);
}