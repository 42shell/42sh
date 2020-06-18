/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/06 18:45:03 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/11 00:51:08 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arith.h"

void	*arith_parse_error(void)
{
	g_arith_status = ERR;
	return (NULL);
}

t_node	*node_new(t_token *token)
{
	t_node *node;

	node = ft_xmalloc(sizeof(t_node));
	node->token = token;
	return (node);
}

void	node_del(t_node **node)
{
	if (*node == NULL)
		return ;
	if ((*node)->left)
		node_del(&(*node)->left);
	if ((*node)->right)
		node_del(&(*node)->right);
	ft_dstr_del(&(*node)->token->value);
	free((*node)->token);
	free(*node);
	*node = NULL;
}

void	discard_token(void)
{
	t_token *tmp;

	tmp = g_cur_tok;
	g_cur_tok = g_cur_tok->next;
	token_del(&tmp);
}
