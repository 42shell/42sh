/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_pad.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:07:44 by nbousset          #+#    #+#             */
/*   Updated: 2019/04/15 01:08:47 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	pf_pad_precision(t_printf *pf)
{
	pf->str = ft_strpad(pf->str, pf->precision, 1, '0');
	pf->precision = 0;
}

void	pf_pad_field_width(t_printf *pf)
{
	if (pf->flags & MINUS_FLAG)
		pf->str = ft_strpad(pf->str, pf->field_width, -1, ' ');
	else if (pf->flags & EQU_FLAG)
		pf->str = ft_strpad(pf->str, pf->field_width, 0, ' ');
	else
	{
		if (pf->flags & ZERO_FLAG)
			pf->str = ft_strpad(pf->str, pf->field_width, 1, '0');
		else
			pf->str = ft_strpad(pf->str, pf->field_width, 1, ' ');
	}
	pf->field_width = 0;
}
