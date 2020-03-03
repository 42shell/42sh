/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 08:48:18 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/14 17:31:49 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		parse_error(int code, char *near)
{
	if (g_parser.error)
		return (0);
	else if (code == HEREDOC_NO_DELIM)
		ft_dprintf(2,
		"42sh: warning: here-document delimited by end-of-file (wanted '%s')\n",
		near);
	else if (code != SILENT_ABORT && near)
		ft_dprintf(2,
		"42sh: syntax error near unexpected token '%s'\n",
		near);
	g_parser.error = code;
	free(near);
	return (0);
}
