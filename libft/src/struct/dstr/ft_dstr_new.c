/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dstr_new.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 23:03:31 by fratajcz          #+#    #+#             */
/*   Updated: 2019/11/21 23:06:12 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_dstr		*ft_dstr_new(char *str, size_t len, size_t size)
{
	t_dstr	*dstr;
	size_t	cmp;

	if (!str)
		return (NULL);
	dstr = (t_dstr *)ft_xmalloc(sizeof(t_dstr));
	if ((cmp = ft_strlen(str)) < len)
		len = cmp;
	if (size - 1 < len)
		size = ft_next_power_of_two(len + 1);
	else
		size = ft_next_power_of_two(size);
	dstr->str = (char *)ft_xmalloc(size);
	ft_memcpy(dstr->str, str, len);
	dstr->size = size;
	dstr->len = len;
	return (dstr);
}
