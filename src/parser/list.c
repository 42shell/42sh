/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/01 16:08:01 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** the following rules should be enough to get a fine shell scripting:
**
** compound_command : brace_group
** 					| subshell
**					| if_clause
**					| while_clause
** 
** and corresponding sub rules
*/

/* ************************************************************************** */

/*
** list				: and_or
** 					| and_or separator_op list
**
** -a newline_list() call is necessary to avoid parse errors
**  in case of empty command "\n" or ending separators "ls;".
**  Normally it should be in compound_command rule
*/

t_node			*parse_list(void)
{
	t_node		*list;
	t_node		*node;

	parse_newline_list();
	if (!g_parser.token
	|| !(list = parse_and_or()))
		return (NULL);
	if (!(node = parse_separator_op()))
	{
		node = (t_node *)ft_xmalloc(sizeof(t_node));
		node->type = NODE_ROOT;
	}
	node->left = list;
	node->right = parse_list();
	if (g_parser.error)
		ast_del(&node);
	list = node;
	return (list);
}
