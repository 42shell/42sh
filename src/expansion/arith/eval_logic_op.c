/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_logic_op.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/06 19:24:02 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/11 01:44:39 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arith.h"

long		eval_or_node(t_node *node)
{
	return (eval_arith_ast(node->left) || eval_arith_ast(node->right));
}

long		eval_and_node(t_node *node)
{
	return (eval_arith_ast(node->left) && eval_arith_ast(node->right));
}

long		eval_not_node(t_node *node)
{
	return (!eval_arith_ast(node->right));
}
