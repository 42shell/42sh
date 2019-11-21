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

#include "../../inc/libft.h"

char	ft_isprime(long n)
{
	long	i;

	if (n == 0 || n == 1)
		return (0);
	i = n / 2;
	while (i > 1)
	{
		if (n % i == 0)
			return (0);
		i--;
	}
	return (1);
}
