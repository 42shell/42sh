/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 19:07:38 by nbousset          #+#    #+#             */
/*   Updated: 2019/05/11 20:42:48 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

int		ft_dtoc_36(int d)
{
	if (d < -35 || d > 35)
		return (-1);
	if (d < 0)
		d = -d;
	if (d < 10)
		return (d + '0');
	else
		return (d + 'A' - 10);
}

char	*ft_utoa_base(uintmax_t n, unsigned int base)
{
	uintmax_t		tmp;
	char			*result;
	size_t			len;

	tmp = n;
	len = 1;
	while (tmp /= base)
		len++;
	if (base < 2 || base > 36)
		return (result = NULL);
	if ((result = (char *)ft_memalloc(len + 1)) == NULL)
		return (result = NULL);
	if (n == 0)
		result[0] = '0';
	while (n)
	{
		result[--len] = ft_dtoc_36(n % base);
		n /= (int)base;
	}
	return (result);
}
