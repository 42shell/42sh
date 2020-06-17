/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_end.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/08 18:31:05 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 18:32:08 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

//ls\n
//ls\0
int			lx_end(void)
{
	if (!g_lexer.line[g_lexer.i] || g_lexer.nl_found)
	{
		g_lexer.end_of_input = 1;
		if (g_lexer.token && !g_lexer.quote_st && g_lexer.nl_found)
			delim_token();
		return (1);
	}
	return (0);
}
