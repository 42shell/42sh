/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ldtoa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:07:44 by nbousset          #+#    #+#             */
/*   Updated: 2019/04/15 01:08:47 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

static char	add_one(char *str)
{
	if (*str == '9')
	{
		*str = '0';
		return (1);
	}
	(*str)++;
	return (0);
}

static char	*ceil(char *str, char decimal)
{
	char	carry;
	size_t	len;

	if (decimal < '5')
		return (str);
	len = ft_strlen(str);
	while (len--)
	{
		if (str[len] == '.')
			continue ;
		carry = add_one(&str[len]);
		if (carry == 0)
			break ;
	}
	if (carry)
		str = ft_strprefix(str, "1", 1);
	return (str);
}

char		*ft_ldtoa(long double lf, size_t precision)
{
	char		*str;
	char		*integer;
	size_t		len;

	integer = ft_utoa((lf < 0.0) ? (uintmax_t)(-lf) : (uintmax_t)(lf));
	len = ft_strlen(integer) + (precision ? precision + 1 : 0);
	if (!(str = (char *)ft_memalloc(len + 1)))
		return (NULL);
	len = ft_strlen(integer);
	ft_memcpy(str, integer, len);
	ft_memdel((void **)&integer);
	str[len++] = precision ? '.' : '\0';
	while (precision--)
	{
		lf *= 10.0L;
		str[len++] = (uintmax_t)(lf) % 10 + '0';
		lf -= (long double)(uintmax_t)(lf);
	}
	if (precision > 17)
		str = ceil(str, (uintmax_t)(lf * 10.0) % 10 + '0');
	return (str);
}
