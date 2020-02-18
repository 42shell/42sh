/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 18:12:45 by nbousset          #+#    #+#             */
/*   Updated: 2019/04/26 18:25:07 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_i(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'A' && c <= 'Z')
		return (c - 'A' + 10);
	if (c >= 'a' && c <= 'z')
		return (c - 'a' + 10);
	return (-1);
}

long	ft_atoi_base(const char *nbr, t_uint32 base)
{
	long	result;
	int		sign;
	int		n;

	result = 0;
	sign = 1;
	while (ft_iswhitespace(*nbr))
		nbr++;
	if (*nbr == '-')
		sign = -1;
	if (*nbr == '-' || *nbr == '+')
		nbr++;
	while ((n = ft_i(*nbr)) > -1 && n < (int)base)
	{
		result = result * (int)base + n;
		nbr++;
	}
	return (result * sign);
}
