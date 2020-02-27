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
                 ;
linebreak        : newline_list
                 | empty 
                 ;
separator_op     : '&'
                 | ';'
                 ;
separator        : separator_op linebreak
                 | newline_list
                 ;
sequential_sep   : ';' linebreak
                 | newline_list
                 ;
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
