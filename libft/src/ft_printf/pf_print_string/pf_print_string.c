/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_print_string.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 12:32:10 by nbousset          #+#    #+#             */
/*   Updated: 2019/05/04 17:01:01 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void		write_char(t_printf *pf, unsigned char c)
{
	if (!(pf->str = (char *)ft_memalloc(2)))
		pf_error(pf, "Dynamic allocation error.\n");
	ft_memcpy(pf->str, &c, 1);
	if (c == 0 && pf->field_width > 0)
		pf->field_width--;
	pf_pad_field_width(pf);
	if (pf->str)
		pf_bufferize(pf, pf->str, ft_strlen(pf->str));
	else
		pf_error(pf, "Dynamic allocation error.\n");
}

static size_t	get_strlen(t_printf *pf, const char *str)
{
	size_t	len;

	len = 0;
	if (pf->flags & PREC_FLAG)
	{
		while (len < pf->precision && *str)
			len++;
	}
	else
		len = ft_strlen(str);
	return (len);
}

static void		write_string(t_printf *pf, const char *str)
{
	size_t	len;

	if (str == NULL)
	{
		pf_bufferize(pf, "(null)", 6);
		return ;
	}
	len = get_strlen(pf, str);
	if (!(pf->str = (char *)ft_memalloc(len + 1)))
		pf_error(pf, "Dynamic allocation error.\n");
	ft_memcpy(pf->str, str, len);
	pf_pad_field_width(pf);
	if (pf->str)
		pf_bufferize(pf, pf->str, ft_strlen(pf->str));
	else
		pf_error(pf, "Dynamic allocation error.\n");
}

void			pf_print_string(t_printf *pf)
{
	char			*str;
	unsigned char	c;

	if (ft_strchr("sS", pf->conv))
	{
		str = va_arg(*pf->args, char *);
		write_string(pf, str);
	}
	else
	{
		if (pf->conv == '%')
			c = '%';
		else
			c = (unsigned char)va_arg(*pf->args, int);
		write_char(pf, c);
	}
	ft_memdel((void **)&pf->str);
	pf->str = NULL;
	pf->len_modif = 0;
	pf->flags = 0;
}
