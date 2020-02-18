/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dstr_cat.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 23:03:42 by fratajcz          #+#    #+#             */
/*   Updated: 2019/11/21 23:05:58 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_dstrcat(t_dstr *dstr, char *str)
{
	char	*tmp;
	size_t	len;
	size_t	new_len;

	if (!dstr || !str)
		return (0);
	len = ft_strlen(str);
	new_len = dstr->len + len;
	if (new_len > dstr->size - 1)
	{
		tmp = dstr->str;
		dstr->size = ft_next_power_of_two(new_len + 1);
		dstr->str = (char *)ft_memrealloc(dstr->str, dstr->len, dstr->size);
		free(tmp);
		if (!dstr->str)
			return (-1);
	}
	ft_memcpy(&dstr->str[dstr->len], str, len);
	dstr->len += len;
	return (len);
}
