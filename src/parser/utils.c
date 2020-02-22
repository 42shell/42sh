/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 08:48:18 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/14 17:31:49 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_token	*node_token(t_node *node)
{
	return ((t_token *)(node->data));
}

/*
**void	print_ast(t_node *ast, int indent_level)
**{
**	int i;
**	int	n;
**
**	n = 0;
**	while (n++ < indent_level)
**		write(1, "  ", 2);
**	if (ast == NULL)
**		return ((void)ft_printf("NULL NODE"));
**	if (ast->data != NULL)
**		ft_printf("%s\n", ((t_token *)ast->data)->value->str);
**	else
**		ft_printf("NULL DATA\n");
**	fflush(stdout);
**	if (ast->nb_children > 0)
**	{
**		i = 0;
**		while (i < ast->nb_children)
**			print_ast(ast->child[i++], indent_level + 1);
**	}
**}
*/

void	free_ast_nodes(t_node *node, bool par_is_pattern)
{
	int		i;
	bool	cur_is_pattern;

	cur_is_pattern = false;
	if (node == NULL)
		return ;
	if (node->data != NULL && !par_is_pattern)
	{
		cur_is_pattern = (node_token(node)->type == PATTERN);
		ft_dstr_del((void **)&node_token(node)->value);
		free(node->data);
	}
	else if (par_is_pattern)
		array_destroy(node->data);
	i = 0;
	while (i < node->nb_children)
		free_ast_nodes(node->child[i++], cur_is_pattern);
	free(node->child);
	free(node);
}
