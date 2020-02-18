/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bufferize_nbr.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 12:32:10 by nbousset          #+#    #+#             */
/*   Updated: 2019/05/04 17:01:01 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_bufferize_nbr(t_buff *buff, long nbr, t_uint32 base, int prefix)
{
	char	*data;

	if (!buff)
		return ;
	if ((base == 2 || base == 16) && prefix == 1)
		ft_bufferize(buff, base == 2 ? "0b" : "0x", 2);
	data = ft_itoa_base(nbr, base);
	ft_bufferize(buff, data, 0);
	ft_memdel((void **)&data);
}
