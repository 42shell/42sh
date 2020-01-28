/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsuffix.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:07:44 by nbousset          #+#    #+#             */
/*   Updated: 2019/04/15 01:08:47 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

char	*ft_strsuffix(char *str, char *suffix, int alloc)
{
	size_t	str_len;
	size_t	suffix_len;

	if (!str)
		return (NULL);
	if (!suffix)
		return (str);
	str_len = ft_strlen(str);
	suffix_len = ft_strlen(suffix);
	if (alloc && !(str = (char *)ft_memrealloc(str, str_len,
													str_len + suffix_len + 1)))
		return (NULL);
	ft_memcpy(str + str_len, suffix, suffix_len);
	return (str);
}
