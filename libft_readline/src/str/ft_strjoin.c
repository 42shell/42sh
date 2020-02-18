/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/07 15:58:30 by nbousset          #+#    #+#             */
/*   Updated: 2019/05/04 16:09:48 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*s3;
	size_t	size1;
	size_t	size2;

	if (!s1 && !s2)
		return (NULL);
	else if (!s1)
		return ((char *)s2);
	else if (!s2)
		return ((char *)s1);
	size1 = ft_strlen(s1);
	size2 = ft_strlen(s2);
	if (!(s3 = ft_memalloc(size1 + size2 + 1)))
		return (NULL);
	ft_memcpy(s3, s1, size1);
	ft_memcpy(&s3[size1], s2, size2);
	return (s3);
}
