/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprime.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/10 18:20:57 by nbousset          #+#    #+#             */
/*   Updated: 2019/04/15 01:09:29 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	ft_isprime(long n)
{
	int		i;
	int		step;
	int		limit;

	if (n < 2)
		return (0);
	if (n % 2 == 0)
		return (n == 2);
	if (n % 3 == 0)
		return (n == 3);
	i = 5;
	step = 4;
	limit = (int)ft_sqrt(n) + 1;
	while (i < limit)
	{
		if (n % i == 0)
			return (0);
		step = 6 - step;
		i += step;
	}
	return (1);
}
