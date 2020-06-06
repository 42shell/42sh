/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_rel_op.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/06 19:25:23 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/11 01:44:20 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arith.h"

long		eval_less_equal_node(t_node *node)
{
	return (eval_arith_ast(node->left) <= eval_arith_ast(node->right));
}

long		eval_less_node(t_node *node)
{
	return (eval_arith_ast(node->left) < eval_arith_ast(node->right));
}

long		eval_more_node(t_node *node)
{
	return (eval_arith_ast(node->left) > eval_arith_ast(node->right));
}

long		eval_more_equal_node(t_node *node)
{
	return (eval_arith_ast(node->left) >= eval_arith_ast(node->right));
}

long		eval_equalequal_node(t_node *node)
{
	return (eval_arith_ast(node->left) == eval_arith_ast(node->right));
}
