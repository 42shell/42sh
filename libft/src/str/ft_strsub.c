/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/07 15:36:23 by nbousset          #+#    #+#             */
/*   Updated: 2019/04/29 19:10:13 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*sub;

	i = 0;
	if (!s)
		return (NULL);
	if ((sub = (char *)ft_xmalloc(len + 1)) == NULL)
		return (NULL);
	while (i < len)
	{
		*sub = s[start + i];
		sub++;
		i++;
	}
	*sub = '\0';
	return (sub - len);
}
