/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expr2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/06 18:52:52 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/16 16:29:38 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arith.h"

/*
** unary_expression:	MINUS unary_expression
**						PLUS unary_expression
**						| factor
*/

t_node	*parse_unary_expression(void)
{
	t_node	*node;

	if (g_arith_status != ERR && g_cur_tok
			&& (g_cur_tok->type == MINUS || g_cur_tok->type == PLUS))
	{
		node = node_new(g_cur_tok);
		g_cur_tok = g_cur_tok->next;
		if ((node->right = parse_unary_expression()) == NULL)
		{
			node_del(&node);
			return (arith_parse_error());
		}
		return (node);
	}
	return (parse_factor());
}

/*
** mul_expression	: mul_expression MUL_OP unary_expression
**					| unary_expression
*/

t_node	*parse_mul_expression(void)
{
	t_node	*mul_expression;
	t_node	*node;

	if (g_arith_status == ERR || !(mul_expression = parse_unary_expression()))
		return (NULL);
	while (g_cur_tok && (g_cur_tok->type == MUL || g_cur_tok->type == DIV
				|| g_cur_tok->type == MOD))
	{
		node = node_new(g_cur_tok);
		node->left = mul_expression;
		g_cur_tok = g_cur_tok->next;
		if (!(node->right = parse_unary_expression()))
		{
			node_del(&node);
			return (arith_parse_error());
		}
		mul_expression = node;
	}
	return (mul_expression);
}

/*
** sum_expression	: sum_expression SUM_OP mul_expression
**					| mul_expression
*/

t_node	*parse_sum_expression(void)
{
	t_node	*sum_expression;
	t_node	*node;

	if (g_arith_status == ERR || !(sum_expression = parse_mul_expression()))
		return (NULL);
	while (g_cur_tok && (g_cur_tok->type == PLUS || (g_cur_tok->type == MINUS)))
	{
		node = node_new(g_cur_tok);
		node->left = sum_expression;
		g_cur_tok = g_cur_tok->next;
		if (!(node->right = parse_mul_expression()))
		{
			node_del(&node);
			return (arith_parse_error());
		}
		sum_expression = node;
	}
	return (sum_expression);
}

/*
** rel_expression:	rel_expression REL_OP sum_expression | sum_expression
*/

t_node	*parse_rel_expression(void)
{
	t_node	*rel_expression;
	t_node	*node;

	if (g_arith_status == ERR || !(rel_expression = parse_sum_expression()))
		return (NULL);
	while (g_cur_tok && is_rel_op(g_cur_tok->type))
	{
		node = node_new(g_cur_tok);
		node->left = rel_expression;
		g_cur_tok = g_cur_tok->next;
		if (!(node->right = parse_sum_expression()))
		{
			node_del(&node);
			return (arith_parse_error());
		}
		rel_expression = node;
	}
	return (rel_expression);
}

/*
** unary_rel_expression:	NOT unary_rel_expression | rel_expression
*/

t_node	*parse_unary_rel_expression(void)
{
	t_node *node;

	if (!g_cur_tok || g_arith_status == ERR)
		return (NULL);
	if (g_cur_tok->type == NOT)
	{
		node = node_new(g_cur_tok);
		g_cur_tok = g_cur_tok->next;
		node->right = parse_unary_rel_expression();
		if (node->right == NULL)
		{
			node_del(&node);
			return (arith_parse_error());
		}
		return (node);
	}
	return (parse_rel_expression());
}
