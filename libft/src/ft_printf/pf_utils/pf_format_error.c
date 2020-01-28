/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_format_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 12:32:10 by nbousset          #+#    #+#             */
/*   Updated: 2019/05/04 17:01:01 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	print_literal(t_printf *pf, char c)
{
	if (c == '\f')
		pf_bufferize(pf, "\\f", 2);
	else if (c == '\t')
		pf_bufferize(pf, "\\t", 2);
	else if (c == '\n')
		pf_bufferize(pf, "\\n", 2);
	else if (c == '\r')
		pf_bufferize(pf, "\\r", 2);
	else if (c == '\v')
		pf_bufferize(pf, "\\v", 2);
	else
		pf_bufferize(pf, &c, 1);
}

void		pf_format_error(t_printf *pf)
{
	size_t	pad;
	size_t	i;

	i = 0;
	ft_bzero(pf->buff, BUFF_SIZE);
	pf_bufferize(pf, "\"", 1);
	while (pf->format_start[i])
		print_literal(pf, pf->format_start[i++]);
	pf_bufferize(pf, "\"", 1);
	pf_bufferize(pf, "\n", 1);
	pad = pf->format - pf->format_start + 1;
	i = pad;
	pf_bufferize(pf, "\033[0;32m", 7);
	while (i--)
		pf_bufferize(pf, "~", 1);
	pf_bufferize(pf, "^\n", 2);
	pf_bufferize(pf, "\033[0m", 4);
	i = pad - 2;
	while (i--)
		pf_bufferize(pf, " ", 1);
	pf_bufferize(pf, "RTFM!\n", 6);
	pf_error(pf, "");
}
