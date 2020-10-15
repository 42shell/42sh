/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_plusplus_op.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/06 19:26:44 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/11 01:43:58 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arith.h"

long		eval_plusplus_node(t_node *node)
{
	char	buf[12];
	long	tmp;

	if (node->right)
	{
		tmp = eval_arith_ast(node->right) + 1;
		ft_itoa(tmp, buf);
		set_var(node->right->token->value->str, buf, 0);
		return (tmp);
	}
	tmp = eval_arith_ast(node->left);
	ft_itoa(tmp + 1, buf);
	set_var(node->left->token->value->str, buf, 0);
	return (tmp);
}

long		eval_minusminus_node(t_node *node)
{
	char	buf[12];
	long	tmp;

	if (node->right)
	{
		tmp = eval_arith_ast(node->right) - 1;
		ft_itoa(tmp, buf);
		set_var(node->right->token->value->str, buf, 0);
		return (tmp);
	}
	tmp = eval_arith_ast(node->left);
	ft_itoa(tmp - 1, buf);
	set_var(node->left->token->value->str, buf, 0);
	return (tmp);
}
