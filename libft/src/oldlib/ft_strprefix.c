/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strprefix.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:07:44 by nbousset          #+#    #+#             */
/*   Updated: 2019/04/15 01:08:47 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

char	*ft_strprefix(char *str, char *prefix, int alloc)
{
	size_t	str_len;
	size_t	prefix_len;

	if (!str)
		return (NULL);
	if (!prefix)
		return (str);
	str_len = ft_strlen(str);
	prefix_len = ft_strlen(prefix);
	if (alloc && !(str = (char *)ft_memrealloc(str, str_len,
													str_len + prefix_len + 1)))
		return (NULL);
	ft_memmove(str + prefix_len, str, str_len);
	ft_memcpy(str, prefix, prefix_len);
	return (str);
}
