/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_handle_color.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 12:32:10 by nbousset          #+#    #+#             */
/*   Updated: 2019/05/04 17:01:01 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	write_color(t_printf *pf, char color[11])
{
	char	color_code[11];

	ft_bzero(color_code, 11);
	if (ft_strequ(color, "red") || ft_strequ(color, "RED"))
		ft_memcpy(color_code, "\033[0;31m", 7);
	else if (ft_strequ(color, "green") || ft_strequ(color, "GREEN"))
		ft_memcpy(color_code, "\033[0;32m", 7);
	else if (ft_strequ(color, "yellow") || ft_strequ(color, "YELLOW"))
		ft_memcpy(color_code, "\033[0;33m", 7);
	else if (ft_strequ(color, "blue") || ft_strequ(color, "BLUE"))
		ft_memcpy(color_code, "\033[0;34m", 7);
	else if (ft_strequ(color, "magenta") || ft_strequ(color, "MAGENTA"))
		ft_memcpy(color_code, "\033[0;35m", 7);
	else if (ft_strequ(color, "cyan") || ft_strequ(color, "CYAN"))
		ft_memcpy(color_code, "\033[0;36m", 7);
	else if (ft_strequ(color, "eoc"))
		ft_memcpy(color_code, "\033[0m", 4);
	if (ft_isupper(color[0]))
		color_code[2] = '1';
	pf_bufferize(pf, color_code, ft_strlen(color_code));
}

static void	read_color(char buff[11], t_printf *pf)
{
	size_t		i;

	i = 0;
	pf->format++;
	while (*pf->format && *pf->format != '}' && i < 10)
	{
		buff[i] = *pf->format;
		pf->format++;
		i++;
	}
	buff[i] = '\0';
	while (*pf->format && *pf->format != '}')
		pf->format++;
	pf->format++;
}

void		pf_handle_color(t_printf *pf)
{
	char		color[11];

	ft_bzero(color, 11);
	read_color(color, pf);
	write_color(pf, color);
}
