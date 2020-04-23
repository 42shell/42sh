/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_buffer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 12:32:10 by nbousset          #+#    #+#             */
/*   Updated: 2019/05/04 17:01:01 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	pf_bufferize(t_printf *pf, const char *data, size_t size)
{
	size_t	diff;

	if (!data || !pf)
		return ;
	diff = PF_BUFF_SIZE - pf->buff_index;
	while (diff < size)
	{
		ft_memcpy(&pf->buff[pf->buff_index], data, diff);
		write(pf->fd, pf->buff, PF_BUFF_SIZE);
		ft_bzero(pf->buff, PF_BUFF_SIZE);
		size -= diff;
		data += diff;
		diff = PF_BUFF_SIZE;
		pf->buff_index = 0;
		pf->ret += PF_BUFF_SIZE;
	}
	ft_memcpy(&pf->buff[pf->buff_index], data, size);
	pf->buff_index += size;
}
