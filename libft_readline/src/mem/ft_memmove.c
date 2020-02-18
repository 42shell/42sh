/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 19:47:16 by nbousset          #+#    #+#             */
/*   Updated: 2019/04/14 01:04:44 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	*ft_memrcpy(void *dst, const void *src, size_t n)
{
	char	*pdst;
	char	*psrc;

	pdst = (char *)dst + n - 1;
	psrc = (char *)src + n - 1;
	while (pdst >= (char *)dst)
	{
		*pdst = *psrc;
		pdst--;
		psrc--;
	}
	return (dst);
}

void		*ft_memmove(void *dst, const void *src, size_t len)
{
	if (src > dst)
		ft_memcpy(dst, src, len);
	if (src < dst)
		ft_memrcpy(dst, src, len);
	return (dst);
}
