/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 04:37:16 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/19 17:21:29 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
newline_list     :  NEWLINE
                 |  NEWLINE newline_list
*/

void		newline_list(void)
{
	if (g_parser.error)
		return ;
	while (g_parser.token && g_parser.token->type == NEWLINE)
	{
		token_del(&g_parser.token);
		g_parser.token = get_next_token();
	}
}

/*
linebreak        : newline_list
                 | empty 
*/

void		linebreak(void)
{
	if (g_parser.error)
		return ;
	if (g_parser.token)
		newline_list();
}

/*
separator_op     : '&'
                 | ';'
*/

t_token		*separator_op(void)
{
	t_token	*sep;

	if (g_parser.error)
		return (NULL);
	if (g_parser.token
	&& (g_parser.token->type == SEMI || g_parser.token->type == AMPERSAND))
	{
		sep = g_parser.token;
		g_parser.token = get_next_token();
		return (sep);
	}
	return (NULL);
}

/*
separator        : separator_op linebreak
                 | newline_list
*/

t_token		*separator(void)
{
	t_token	*sep;

	if (g_parser.error)
		return (NULL);
	if (g_parser.token)
	{
		if ((sep = separator_op()))
		{
			linebreak();
			return (sep);
		}
		newline_list();
	}
	return (NULL);
}
