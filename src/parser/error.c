/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 08:48:18 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/14 17:31:49 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		ps_heredoc_eof(char *delim)
{
	ft_dprintf(2,
	"42sh: warning: here-document delimited by end-of-file (wanted '%.*s')\n",
	(int)(ft_strlen(delim) - 1), delim);
	g_parser.error = NOERR;
	return (0);
}

int		ps_error(char *near)
{
	//g_last_exit_st = g_parser.error?
	if (g_parser.error != SILENT_ABORT)
		ft_dprintf(2,
		"42sh: syntax error near unexpected token '%s'\n",
		near);
	g_parser.error = NOERR;
	return (0);
}