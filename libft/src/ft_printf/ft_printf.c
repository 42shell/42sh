/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:07:44 by nbousset          #+#    #+#             */
/*   Updated: 2019/04/15 01:08:47 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	handle_option(t_printf *pf)
{
	pf->format++;
	if (*pf->format == '\0')
		return (-1);
	if (*pf->format == '{')
		pf_handle_color(pf);
	else
	{
		if ((pf_parse_arg(pf)) < 0)
			return (-1);
		if (pf->conv)
			pf_handle_arg(pf);
	}
	return (1);
}

static int	process(t_printf *pf)
{
	while (*pf->format)
	{
		if (*pf->format == '%')
		{
			if ((handle_option(pf)) < 0)
				return (-1);
		}
		if (*pf->format == '%')
			continue;
		if (*pf->format == '\0')
			break ;
		else
			pf_bufferize(pf, pf->format, 1);
		pf->format++;
	}
	write(pf->fd, pf->buff, pf->buff_index);
	pf->ret += pf->buff_index;
	pf->buff_index = 0;
	return (pf->ret);
}

int			ft_dprintf(int fd, const char *format, ...)
{
	t_printf	pf;
	va_list		args;
	int			ret;

	va_start(args, format);
	pf_init(&pf, format, &args, fd);
	ret = process(&pf);
	va_end(args);
	pf_del(&pf);
	return (ret);
}

int			ft_printf(const char *format, ...)
{
	t_printf	pf;
	va_list		args;
	int			ret;

	va_start(args, format);
	pf_init(&pf, format, &args, 1);
	ret = process(&pf);
	va_end(args);
	pf_del(&pf);
	return (ret);
}
