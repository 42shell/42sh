/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_del.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:07:44 by nbousset          #+#    #+#             */
/*   Updated: 2019/04/15 01:08:47 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void		pf_del(t_printf *pf)
{
	ft_bzero(pf, sizeof(t_printf));
	ft_memdel((void **)&pf->str);
	ft_memdel((void **)&pf->format);
	pf->str = NULL;
	pf->format_start = NULL;
	pf->format = NULL;
	pf->args = NULL;
	va_end(*pf->args);
}
