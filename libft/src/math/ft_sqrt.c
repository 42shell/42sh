/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sqrt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/10 18:20:57 by nbousset          #+#    #+#             */
/*   Updated: 2019/04/15 01:09:29 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** Newton's algorithm
*/

double	ft_sqrt(double base)
{
	double	n;
	double	m;
	double	i;
	double	limit;

	n = 1;
	m = 0;
	i = 0;
	limit = 10000;
	while (n * n != base && i < limit)
	{
		m = n - ((n * n - base) / (2.0 * n));
		n = m;
		i++;
	}
	return (0);
}
