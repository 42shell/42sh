/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dstr_from_str.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 23:03:42 by fratajcz          #+#    #+#             */
/*   Updated: 2019/11/21 23:05:58 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_dstr	*ft_dstr_from_str(char *str)
{
	t_dstr	*dstr;
	size_t	size;
	size_t	len;

	if (!(dstr = (t_dstr *)ft_memalloc(sizeof(t_dstr))))
		return (NULL);
	len = ft_strlen(str);
	size = ft_next_power_of_two(len + 1);
	if (!(dstr->str = (char *)ft_memalloc(size)))
	{
		free(dstr);
		return (NULL);
	}
	ft_memcpy(dstr->str, str, len);
	dstr->size = size;
	dstr->len = len;
	return (dstr);
}
