/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nbrlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/21 10:01:03 by nbousset          #+#    #+#             */
/*   Updated: 2019/06/13 22:29:39 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_nbrlen(long n, uint32_t base)
{
	size_t	len;

	len = 0;
	if (base < 2)
		return (0);
	if ((n < 0 && base == 10) || n == 0)
		len++;
	while (n != 0)
	{
		n = n / (long)base;
		len++;
	}
	return (len);
}
