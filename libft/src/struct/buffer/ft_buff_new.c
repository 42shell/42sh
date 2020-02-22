/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_buff_new.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 12:32:10 by nbousset          #+#    #+#             */
/*   Updated: 2019/05/04 17:01:01 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_buff			*ft_buff_new(size_t buff_size, int fd,
											void (*flush_func)(t_buff *buff))
{
	t_buff		*buff;

	if (buff_size == 0)
		return (NULL);
	if (!(buff = (t_buff *)ft_memalloc(sizeof(t_buff))))
		return (NULL);
	if (!(buff->bytes = (unsigned char *)ft_memalloc(buff_size)))
	{
		ft_memdel((void *)&buff);
		return (NULL);
	}
	buff->size = buff_size;
	buff->i = 0;
	buff->flush_func = flush_func;
	buff->fd = fd < 0 ? 1 : fd;
	return (buff);
}
