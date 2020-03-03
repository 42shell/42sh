/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:52:31 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/07 00:36:28 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		redir_error(int code)
{
	if (code == ERROR_REDIR_OPEN)
		ft_dprintf(2, "42sh: Could not open file\n");
	else if (code == ERROR_REDIR_BAD_FD)
		ft_dprintf(2, "42sh: Bad file descriptor\n");
	return (code);
}
