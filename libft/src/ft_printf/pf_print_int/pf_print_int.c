/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_int.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 12:32:10 by nbousset          #+#    #+#             */
/*   Updated: 2019/05/04 17:01:01 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void		prefix_int(t_printf *pf, intmax_t i)
{
	if (i < 0)
		pf->str = ft_strprefix(pf->str, "-", 1);
	else if (pf->flags & PLUS_FLAG)
		pf->str = ft_strprefix(pf->str, "+", 1);
	else if (pf->flags & SPACE_FLAG)
		pf->str = ft_strprefix(pf->str, " ", 1);
	else
		return ;
}

static void		write_int(t_printf *pf, intmax_t i)
{
	uintmax_t	u;

	u = (i < 0) ? -i : i;
	if ((pf->flags & ZERO_FLAG) && (pf->flags & PREC_FLAG))
		pf->flags ^= ZERO_FLAG;
	pf->str = (i == 0 && (pf->flags & PREC_FLAG) && pf->precision == 0)
												? ft_strnew(0) : ft_utoa(u);
	if (pf->str)
		pf_pad_precision(pf);
	if (pf->flags & ZERO_FLAG && pf->str)
	{
		if (i < 0 || (pf->flags & PLUS_FLAG) || (pf->flags & SPACE_FLAG))
		{
			pf->field_width -= (pf->field_width ? 1 : 0);
			pf_pad_field_width(pf);
		}
	}
	if (pf->str)
		prefix_int(pf, i);
	if (pf->str)
		pf_pad_field_width(pf);
	if (pf->str)
		pf_bufferize(pf, pf->str, ft_strlen(pf->str));
	else
		pf_error(pf, "Dynamic allocation error.\n");
}

static intmax_t	cast_int(t_printf *pf)
{
	intmax_t	i;

	if (pf->len_modif & HH_LEN)
		i = (char)va_arg(*pf->args, int);
	else if (pf->len_modif & H_LEN)
		i = (short)va_arg(*pf->args, int);
	else if (pf->len_modif & L_LEN)
		i = va_arg(*pf->args, long);
	else if (pf->len_modif & LL_LEN)
		i = va_arg(*pf->args, long long);
	else if (pf->len_modif & J_LEN)
		i = va_arg(*pf->args, intmax_t);
	else if (pf->len_modif & Z_LEN)
		i = va_arg(*pf->args, ssize_t);
	else
		i = va_arg(*pf->args, int);
	return (i);
}

void			pf_print_int(t_printf *pf)
{
	intmax_t	i;

	i = cast_int(pf);
	write_int(pf, i);
	ft_memdel((void **)&pf->str);
	pf->str = NULL;
	pf->len_modif = 0;
	pf->flags = 0;
}
