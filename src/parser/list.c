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

t_list			*ps_list(void)
{
	t_list		*list;
	t_node		*and_or;
	t_token		*sep;

	ps_newline_list();
	if (!g_parser.token
	|| !(and_or = ps_and_or()))
		return (NULL);
	list = (t_list *)ft_xmalloc(sizeof(t_list));
	list->ast = and_or;
	if ((sep = ps_separator_op()))
	{
		list->bg = (sep->type == AMPERSAND);
		token_del(&sep);
		list->next = ps_list();
		if (g_parser.error)
			list_del(&list);
	}
	return (list);
}