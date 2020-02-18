/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dstrnew.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 23:03:31 by fratajcz          #+#    #+#             */
/*   Updated: 2019/11/21 23:06:12 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_dstr		*ft_dstrnew(size_t size)
{
	t_dstr	*dstr;

	if (!(dstr = (t_dstr *)ft_memalloc(sizeof(t_dstr))))
		return (NULL);
	size = ft_next_power_of_two(size);
	if (!(dstr->str = (char *)ft_memalloc(size)))
	{
		free(dstr);
		return (NULL);
	}
	dstr->size = size;
	return (dstr);
}
