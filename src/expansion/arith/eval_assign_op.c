/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_assign_op.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/06 19:26:10 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/11 02:34:26 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arith.h"

long		eval_assign_node(t_node *node)
{
	char	buf[12];

	ft_itoa(eval_arith_ast(node->right), buf);
	set_var(node->left->token->value->str, buf, 0);
	return (ft_atoi(buf));
}

long		eval_add_assign_node(t_node *node)
{
	char	buf[12];

	ft_itoa(eval_arith_ast(node->left) + eval_arith_ast(node->right), buf);
	set_var(node->left->token->value->str, buf, 0);
	return (ft_atoi(buf));
}

long		eval_sub_assign_node(t_node *node)
{
	char	buf[12];

	ft_itoa(eval_arith_ast(node->left) - eval_arith_ast(node->right), buf);
	set_var(node->left->token->value->str, buf, 0);
	return (ft_atoi(buf));
}

long		eval_mul_assign_node(t_node *node)
{
	char	buf[12];

	ft_itoa(eval_arith_ast(node->left) * eval_arith_ast(node->right), buf);
	set_var(node->left->token->value->str, buf, 0);
	return (ft_atoi(buf));
}

long		eval_div_assign_node(t_node *node)
{
	char	buf[12];
	long	tmp;

	tmp = eval_arith_ast(node->right);
	if (tmp == 0)
	{
		g_arith_status = ERR;
		return (0);
	}
	ft_itoa(eval_arith_ast(node->left) / tmp, buf);
	set_var(node->left->token->value->str, buf, 0);
	return (ft_atoi(buf));
}
