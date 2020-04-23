/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bufferize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 12:32:10 by nbousset          #+#    #+#             */
/*   Updated: 2019/05/04 17:01:01 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_bufferize(t_buff *buff, const char *data, size_t data_size)
{
	size_t	diff;

	if (!buff || !data)
		return ;
	if (data_size == 0)
		data_size = ft_strlen(data);
	diff = buff->size - buff->i;
	while (diff < data_size)
	{
		ft_memcpy(&buff->bytes[buff->i], data, diff);
		buff->i += diff;
		ft_buff_flush(buff);
		data += diff;
		data_size -= diff;
		diff = buff->size;
	}
	ft_memcpy(&buff->bytes[buff->i], data, data_size);
	buff->i += data_size;
}
