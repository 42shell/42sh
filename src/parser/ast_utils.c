/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 08:48:18 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/14 17:31:49 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			ast_del(t_ast **ast_list)
{
	if (!ast_list || !*ast_list)
		return (0);
	free_ast_nodes((*ast_list)->root, false);
	ast_del(&((*ast_list)->next));
	ft_memdel((void **)ast_list);
	return (0);
}

t_ast		*ast_new(void)
{
	return ((t_ast *)ft_xmalloc(sizeof(t_ast)));
}

void		free_ast_nodes(t_node *node, bool par_is_pattern)
{
	int		i;
	//bool	cur_is_pattern;

	(void)par_is_pattern;
	//cur_is_pattern = false;
	if (node == NULL)
		return ;
	else if (node->type == NODE_COMMAND)
		command_del((t_command **)&node->data);
	else if (node->data != NULL /*&& !par_is_pattern*/)
	{
		token_del((t_token **)&node->data);
		//cur_is_pattern = (node_token(node)->type == PATTERN);
	}
	//else if (par_is_pattern)
	//	array_destroy(node->data);
	i = 0;
	while (i < node->nb_children)
		free_ast_nodes(node->child[i++], /*cur_is_pattern*/ 0);
	free(node->child);
	free(node);
}
