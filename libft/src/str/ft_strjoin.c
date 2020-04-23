/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/07 15:58:30 by nbousset          #+#    #+#             */
/*   Updated: 2020/01/11 13:07:50 by fratajcz         ###   ########.fr       */
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
		return (ft_strdup(s2));
	else if (!s2)
		return (ft_strdup(s1));
	size1 = ft_strlen(s1);
	size2 = ft_strlen(s2);
	if (!(s3 = ft_xmalloc(size1 + size2 + 1)))
		return (NULL);
	ft_memcpy(s3, s1, size1);
	ft_memcpy(&s3[size1], s2, size2);
	return (s3);
}
