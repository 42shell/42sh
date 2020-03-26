/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and_or.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/13 18:32:06 by fratajcz         ###   ########.fr       */
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

t_node			*ps_list(void)
{
	t_node		*list;
	t_node		*node;

	ps_newline_list();
	if (!g_parser.token
	|| !(list = ps_and_or()))
		return (NULL);
	while (list && (node = ps_separator_op()))
	{
		node->left = list;
		ps_newline_list();
		node->right = ps_and_or();
		if (g_parser.error)
			ast_del(&node);
		list = node;
	}
	return (list);
}