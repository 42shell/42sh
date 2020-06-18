/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_factor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/06 18:50:24 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/10 21:44:10 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arith.h"

/*
** mutable : ID
*/

t_node	*parse_mutable(void)
{
	t_node *node;

	if (g_arith_status != ERR && g_cur_tok && g_cur_tok->type == ID)
	{
		node = node_new(g_cur_tok);
		g_cur_tok = g_cur_tok->next;
		return (node);
	}
	return (NULL);
}

/*
** immutable:	LPAREN expression RPAREN
**				| NUM
*/

t_node	*parse_immutable(void)
{
	t_node *node;

	if (g_cur_tok == NULL || g_arith_status == ERR)
		return (NULL);
	if (g_cur_tok->type == LPAREN)
	{
		discard_token();
		g_paren_lvl++;
		node = parse_expression();
		g_paren_lvl--;
		if (!node || !g_cur_tok || g_cur_tok->type != RPAREN)
		{
			node_del(&node);
			return (arith_parse_error());
		}
		discard_token();
		return (node);
	}
	if (g_cur_tok->type == NUM)
	{
		node = node_new(g_cur_tok);
		g_cur_tok = g_cur_tok->next;
		return (node);
	}
	return (NULL);
}

/*
** factor:	immutable | mutable
**				| mutable PLUSPLUS
**				| mutable MINMIN
**				| PLUSPLUS mutable
**				| MINMIN mutable
*/

t_node	*parse_factor(void)
{
	t_node	*node;
	t_node	*tmp;

	if ((node = parse_immutable()))
		return (node);
	if (g_cur_tok && (g_cur_tok->type == PLUSPLUS || g_cur_tok->type == MINMIN))
	{
		node = node_new(g_cur_tok);
		g_cur_tok = g_cur_tok->next;
		if ((node->right = parse_mutable()) == NULL)
		{
			node_del(&node);
			return (arith_parse_error());
		}
		return (node);
	}
	if ((node = parse_mutable()) && g_cur_tok
			&& (g_cur_tok->type == PLUSPLUS || g_cur_tok->type == MINMIN))
	{
		tmp = node_new(g_cur_tok);
		g_cur_tok = g_cur_tok->next;
		tmp->left = node;
		return (tmp);
	}
	return (node);
}
