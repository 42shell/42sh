/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:07:44 by nbousset          #+#    #+#             */
/*   Updated: 2019/04/15 01:08:47 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void		pf_error(t_printf *pf, char *message)
{
	write(pf->fd, pf->buff, pf->buff_index);
	pf->ret += pf->buff_index;
	pf->buff_index = 0;
	if (message)
		ft_putstr(message);
	pf_del(pf);
	exit(EXIT_FAILURE);
}
