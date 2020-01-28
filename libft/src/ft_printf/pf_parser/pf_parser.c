/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 12:32:10 by nbousset          #+#    #+#             */
/*   Updated: 2019/05/04 17:01:01 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	parse_len_modif(t_printf *pf)
{
	while (pf_ismodif(*pf->format))
	{
		if (*pf->format == 'h')
		{
			if (pf->len_modif & H_LEN)
				pf->len_modif = ~(pf->len_modif | ~HH_LEN);
			else
				pf->len_modif = ~(pf->len_modif | ~H_LEN);
		}
		else if (*pf->format == 'l')
		{
			if (pf->len_modif & L_LEN)
				pf->len_modif = ~(pf->len_modif | ~LL_LEN);
			else
				pf->len_modif = ~(pf->len_modif | ~L_LEN);
		}
		else if (*pf->format == 'L')
			pf->len_modif = ~(pf->len_modif | ~LD_LEN);
		else if (*pf->format == 'j')
			pf->len_modif = ~(pf->len_modif | ~J_LEN);
		else if (*pf->format == 'z')
			pf->len_modif = ~(pf->len_modif | ~Z_LEN);
		pf->format++;
	}
}

static void	parse_precision(t_printf *pf)
{
	int	precision;

	if (*pf->format == '.')
	{
		pf->format++;
		pf->flags |= PREC_FLAG;
		if (*pf->format == '*')
		{
			precision = va_arg(*pf->args, int);
			if (precision < 0)
				pf->flags ^= PREC_FLAG;
			pf->precision = precision < 0 ? 0 : precision;
			pf->format++;
		}
		if (ft_isdigit(*pf->format))
		{
			pf->precision = ft_atoi(pf->format);
			while (ft_isdigit(*pf->format))
				pf->format++;
		}
	}
}

static void	parse_field_w(t_printf *pf)
{
	int		field_width;

	if (*pf->format == '*')
	{
		field_width = va_arg(*pf->args, int);
		if (field_width < 0)
		{
			pf->flags &= ~ZERO_FLAG;
			pf->flags = (pf->flags | MINUS_FLAG) ^ EQU_FLAG;
		}
		pf->field_width = field_width < 0 ? -field_width : field_width;
		pf->format++;
	}
	if (ft_isdigit(*pf->format))
	{
		pf->field_width = ft_atoi(pf->format);
		while (ft_isdigit(*pf->format))
			pf->format++;
	}
}

static void	parse_flags(t_printf *pf)
{
	while (pf_isflag(*pf->format))
	{
		if (*pf->format == '#')
			pf->flags |= HASH_FLAG;
		else if (*pf->format == '0' && !(pf->flags & MINUS_FLAG))
			pf->flags |= ZERO_FLAG;
		else if (*pf->format == ' ')
			pf->flags |= SPACE_FLAG;
		else if (*pf->format == '-' || *pf->format == '=')
		{
			pf->flags &= ~ZERO_FLAG;
			if (*pf->format == '-')
				pf->flags = (pf->flags | MINUS_FLAG) ^ EQU_FLAG;
			else
				pf->flags = (pf->flags | EQU_FLAG) ^ MINUS_FLAG;
		}
		else if (*pf->format == '+')
		{
			pf->flags &= ~SPACE_FLAG;
			pf->flags |= PLUS_FLAG;
		}
		pf->format++;
	}
}

int			pf_parse_arg(t_printf *pf)
{
	parse_flags(pf);
	parse_field_w(pf);
	parse_precision(pf);
	parse_len_modif(pf);
	while (*pf->format && !pf_isconv(*pf->format))
		pf->format++;
	if (*pf->format)
		pf->conv = *pf->format++;
	else
		return (-1);
	if (ft_isupper(pf->conv))
		pf->flags |= UP_FLAG;
	return (1);
}
