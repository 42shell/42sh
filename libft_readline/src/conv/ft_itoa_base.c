/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 19:07:38 by nbousset          #+#    #+#             */
/*   Updated: 2019/05/11 20:42:48 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_a(int d)
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

char		*ft_itoa_base(long n, unsigned int base)
{
	char			*result;
	size_t			len;

	len = ft_nbrlen(n, base);
	if (base < 2 || base > 36)
		return (NULL);
	if ((result = (char *)ft_memalloc(len + 1)) == NULL)
		return (NULL);
	if (n < 0 && base == 10)
		result[0] = '-';
	if (n == 0)
		result[0] = '0';
	while (n)
	{
		result[--len] = ft_a(n % (int)base);
		n /= (int)base;
	}
	return (result);
}
