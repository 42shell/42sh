/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strpad.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:07:44 by nbousset          #+#    #+#             */
/*   Updated: 2019/04/15 01:08:47 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strpad(char *str, size_t field_width, int side, char c)
{
	size_t	str_len;
	size_t	pad_len;
	char	*tmp;

	if (!str)
		return (NULL);
	if (field_width <= (str_len = ft_strlen(str)))
		return (str);
	pad_len = field_width - str_len;
	tmp = str;
	if (!(str = (char *)ft_memrealloc(str, str_len, field_width + 1)))
		return (NULL);
	free(tmp);
	ft_memset(str + str_len, c, pad_len);
	if (side == 0)
	{
		ft_memmove(str + pad_len / 2, str, str_len);
		ft_memset(str, c, pad_len / 2);
	}
	else if (side == 1)
	{
		ft_memmove(str + pad_len, str, str_len);
		ft_memset(str, c, pad_len);
	}
	return (str);
}
