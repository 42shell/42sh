/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_print_uint.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 12:32:10 by nbousset          #+#    #+#             */
/*   Updated: 2019/05/04 17:01:01 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void			prefix_uint(t_printf *pf, uint32_t base, uintmax_t u)
{
	if ((pf->flags & HASH_FLAG) || (ft_strchr("pP", pf->conv)))
	{
		if (base == 2 && u != 0)
			pf->str = ft_strprefix(pf->str, "0B", 1);
		else if (base == 8 && u != 0 && *pf->str != '0')
			pf->str = ft_strprefix(pf->str, "0", 1);
		else if ((base == 16 && u != 0) || ft_strchr("pP", pf->conv))
			pf->str = ft_strprefix(pf->str, "0X", 1);
		else
			return ;
	}
	if (!(pf->flags & UP_FLAG) && pf->str)
		ft_strtolower(pf->str);
}

static void			write_uint(t_printf *pf, uintmax_t u, uint32_t base)
{
	if ((pf->flags & ZERO_FLAG) && (pf->flags & PREC_FLAG))
		pf->flags ^= ZERO_FLAG;
	pf->str = (u == 0 && (pf->flags & PREC_FLAG) && pf->precision == 0
			&& !(base == 8 && pf->flags & HASH_FLAG)) ?
							ft_strnew(0) : ft_utoa_base(u, base);
	if (pf->str)
		pf_pad_precision(pf);
	if ((pf->flags & HASH_FLAG) && (pf->flags & ZERO_FLAG)
												&& !(pf->flags & MINUS_FLAG))
	{
		if (base != 8 && pf->field_width > 1)
			pf->field_width -= 2;
		else if (base == 8 && pf->field_width > 0)
			pf->field_width -= 1;
		if (pf->str)
			pf_pad_field_width(pf);
	}
	if (pf->str)
		prefix_uint(pf, base, u);
	if (pf->str)
		pf_pad_field_width(pf);
	if (pf->str)
		pf_bufferize(pf, pf->str, ft_strlen(pf->str));
	else
		pf_error(pf, "Dynamic allocation error.\n");
}

static uintmax_t	cast_uint(t_printf *pf)
{
	uintmax_t	u;

	if (pf->conv == 'U')
		u = va_arg(*pf->args, unsigned long);
	else if (pf->len_modif & HH_LEN)
		u = (unsigned char)va_arg(*pf->args, unsigned int);
	else if (pf->len_modif & H_LEN)
		u = (unsigned short)va_arg(*pf->args, unsigned int);
	else if (pf->len_modif & L_LEN)
		u = va_arg(*pf->args, unsigned long);
	else if (pf->len_modif & LL_LEN || ft_strchr("pP", pf->conv))
		u = va_arg(*pf->args, unsigned long long);
	else if (pf->len_modif & J_LEN)
		u = va_arg(*pf->args, unsigned long);
	else if (pf->len_modif & Z_LEN)
		u = va_arg(*pf->args, size_t);
	else
		u = va_arg(*pf->args, unsigned int);
	return (u);
}

void				pf_print_uint(t_printf *pf, uint32_t base)
{
	uintmax_t	u;

	u = cast_uint(pf);
	write_uint(pf, u, base);
	ft_memdel((void **)&pf->str);
	pf->str = NULL;
	pf->len_modif = 0;
	pf->flags = 0;
}
