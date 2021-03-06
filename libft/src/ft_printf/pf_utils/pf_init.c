/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 12:32:10 by nbousset          #+#    #+#             */
/*   Updated: 2019/05/04 17:01:01 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		pf_init(t_printf *pf, const char *format, va_list *args, int fd)
{
	if (fd < 0)
		pf_error(pf, "Wrong fd\n");
	ft_bzero(pf, sizeof(t_printf));
	pf->format_start = format;
	pf->format = format;
	pf->args = args;
	pf->fd = fd;
	pf->str = NULL;
	return (1);
}
