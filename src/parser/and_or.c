/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and_or.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/13 18:32:06 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
and_or           :                         pipeline
                 | and_or AND_IF linebreak pipeline
                 | and_or OR_IF  linebreak pipeline

                                  LIST create a job
                                 /
                           AND_OR                 LIST;
					      /  |   \	               |
                    AND_OR   ||   test          PIPELINE
				   /  |  \                       /    \
                ls    &&   cat             PIPELINE    CMD 
                                                |       |
                                               ls     (cat)
*/

/*
** and_or			: pipeline AND_IF/OR_IF linebreak and_or
**					: pipeline
**
**                 ||
**               /   \
**             &&    test
**           /    \
**         ls     cat
*/

t_node				*ps_and_or(void)
{
	t_node			*and_or;
	t_node			*node;
	
	and_or = ps_pipeline();
	while (g_parser.token->type == AND_IF || g_parser.token->type == OR_IF)
	{
		node = (t_node *)ft_xmalloc(sizeof(t_node));
		node->type = g_parser.token->type;
		node->left = and_or;
		token_del(&g_parser.token);
		g_parser.token = get_next_token();
		ps_linebreak(node->type);
		if (!(node->right = ps_pipeline()))
		{
			g_parser.error = g_parser.error ? g_parser.error : NO_CMD_AFTER_OP;
			ast_del(&node);
		}
		and_or = node;
	}
	return (and_or);
}