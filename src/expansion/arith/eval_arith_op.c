/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_arith_op.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/06 19:09:19 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/11 02:34:46 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arith.h"

long		eval_minus_node(t_node *node)
{
	return (eval_arith_ast(node->left) - eval_arith_ast(node->right));
}

long		eval_plus_node(t_node *node)
{
	return (eval_arith_ast(node->left) + eval_arith_ast(node->right));
}

long		eval_mul_node(t_node *node)
{
	return (eval_arith_ast(node->left) * eval_arith_ast(node->right));
}

long		eval_div_node(t_node *node)
{
	long tmp;

	tmp = eval_arith_ast(node->right);
	if (tmp == 0)
	{
		g_arith_status = ERR;
		return (0);
	}
	return (eval_arith_ast(node->left) / tmp);
}

long		eval_mod_node(t_node *node)
{
	long tmp;

	tmp = eval_arith_ast(node->right);
	if (tmp == 0)
	{
		g_arith_status = ERR;
		return (0);
	}
	return (eval_arith_ast(node->left) % tmp);
}
