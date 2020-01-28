/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_base_isvalid.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/16 20:31:22 by nbousset          #+#    #+#             */
/*   Updated: 2019/05/16 20:31:26 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

uint32_t	ft_base_isvalid(char *base)
{
	uint32_t	b;
	int			i;
	int			j;

	if ((b = ft_strlen(base)) < 2)
		return (0);
	i = 0;
	while (base[i])
	{
		j = 0;
		while (base[j])
		{
			if (j != i && base[j] == base[i])
				return (0);
			j++;
		}
		i++;
	}
	return (b);
}
