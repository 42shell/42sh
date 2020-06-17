/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/10 21:31:31 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/11 02:17:18 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arith.h"
#include "ft_printf.h"

long		eval_num_node(t_node *node)
{
	return (ft_atoi(node->token->value->str));
}

long		eval_not_equal_node(t_node *node)
{
	return (eval_arith_ast(node->left) != eval_arith_ast(node->right));
}

long		eval_id_node(t_node *node)
{
	t_var *var;

	var = ht_get(g_shell.vars, node->token->value->str);
	if (var == NULL)
		return (0);
	return (ft_atoi(var->value));
}

long	(*g_eval_functions[NB_OP]) (t_node *node) = {
	eval_id_node,
	eval_num_node,
	eval_assign_node,
	eval_add_assign_node,
	eval_sub_assign_node,
	eval_mul_assign_node,
	eval_div_assign_node,
	eval_plusplus_node,
	eval_minusminus_node,
	eval_or_node,
	eval_and_node,
	eval_not_node,
	eval_less_equal_node,
	eval_less_node,
	eval_more_node,
	eval_more_equal_node,
	eval_equalequal_node,
	eval_not_equal_node,
	eval_minus_node,
	eval_plus_node,
	eval_mul_node,
	eval_div_node,
	eval_mod_node,
	NULL,
	NULL,
	NULL,
	NULL,
};

int		g_arith_recursion_level;
bool	g_arith_toomuch;

long		eval_arith_ast(t_node *ast)
{
	long		ret;

	if (g_arith_status == ERR || ast == NULL)
		return (0);
	g_arith_recursion_level++;
	if (g_arith_recursion_level > 1024)
		g_arith_toomuch = true;
	if (g_arith_toomuch)
		return (0);
	ret = g_eval_functions[ast->token->type](ast);
	g_arith_recursion_level--;
	return (ret);
}

long		eval_expr(const char *str)
{
	t_token	*token_list;
	t_node	*ast;
	long	ret;

	if (str[0] == '\0')
		return (0);
	if ((token_list = get_token_list(str)) == NULL)
	{
		g_arith_status = ERR;
		ft_dprintf(2, "42sh: %s: invalid math expression\n", str);
		return (0);
	}
	ast = parse(token_list);
	g_arith_recursion_level = 0;
	g_arith_toomuch = false;
	ret = eval_arith_ast(ast);
	if (g_arith_status == ERR || g_arith_toomuch)
	{
		token_list_del(&g_cur_tok);
		ft_dprintf(2, "42sh: %s: invalid math expression\n", str);
	}
	node_del(&ast);
	return (ret);
}
