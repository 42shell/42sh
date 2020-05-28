/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:52:31 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/28 19:58:29 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

extern char	*g_expand_error_token;

int		redir_error(int code)
{
	if (code == ERROR_REDIR_OPEN)
		ft_dprintf(2, "42sh: Could not open file\n");
	else if (code == ERROR_REDIR_BAD_FD)
		ft_dprintf(2, "42sh: Bad file descriptor\n");
	else if (code == ERROR_REDIR_EXPAND)
		ft_dprintf(2, "42sh: %s: bad substitution\n", g_expand_error_token);
	return (code);
}
