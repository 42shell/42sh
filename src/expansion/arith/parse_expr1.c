/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expr1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/06 19:34:10 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/10 21:39:55 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arith.h"

t_token					*g_cur_tok;
enum e_parser_status	g_arith_status = OK;
int						g_paren_lvl;

/*
** and_expression:	and_expression AND unary_rel_expression
**					| unary_rel_expression
*/

t_node	*parse_and_expression(void)
{
	t_node	*and_expression;
	t_node	*node;

	if (g_arith_status == ERR
			|| !(and_expression = parse_unary_rel_expression()))
		return (NULL);
	while (g_cur_tok && g_cur_tok->type == AND)
	{
		node = node_new(g_cur_tok);
		node->left = and_expression;
		g_cur_tok = g_cur_tok->next;
		if (!(node->right = parse_unary_rel_expression()))
		{
			node_del(&node);
			return (arith_parse_error());
		}
		and_expression = node;
	}
	return (and_expression);
}

/*
** simple_expression	: simple_expression OR and_expression
**						| and_expression
*/

t_node	*parse_simple_expression(void)
{
	t_node	*simple_expression;
	t_node	*node;

	if (!(simple_expression = parse_and_expression()) || g_arith_status == ERR)
		return (NULL);
	while (g_cur_tok && g_cur_tok->type == OR)
	{
		node = node_new(g_cur_tok);
		node->left = simple_expression;
		g_cur_tok = g_cur_tok->next;
		if (!(node->right = parse_and_expression()))
		{
			node_del(&node);
			return (arith_parse_error());
		}
		simple_expression = node;
	}
	return (simple_expression);
}

/*
** expression:	mutable ASSIGNMENT expression
**				| simple_expression
*/

t_node	*parse_expression(void)
{
	t_node	*mutable;
	t_node	*node;

	if (g_paren_lvl >= 2048)
		return (arith_parse_error());
	mutable = parse_mutable();
	if (mutable == NULL)
		return (parse_simple_expression());
	if (g_cur_tok && is_assignment(g_cur_tok->type))
	{
		node = node_new(g_cur_tok);
		g_cur_tok = g_cur_tok->next;
		node->left = mutable;
		if ((node->right = parse_expression()) == NULL)
			node_del(&node);
		return (node);
	}
	g_cur_tok = mutable->token;
	free(mutable);
	return (parse_simple_expression());
}

t_node	*parse(t_token *token_list)
{
	t_node *ret;

	g_cur_tok = token_list;
	g_paren_lvl = 0;
	g_arith_status = OK;
	ret = parse_expression();
	if (g_cur_tok != NULL)
		g_arith_status = ERR;
	return (ret);
}
