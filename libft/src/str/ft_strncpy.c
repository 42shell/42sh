/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 18:39:55 by fratajcz          #+#    #+#             */
/*   Updated: 2019/04/05 15:17:39 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

char			*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;
	int		src_over;

	i = 0;
	src_over = 0;
	while (i < n)
	{
		if (!src_over && src[i])
			dest[i] = src[i];
		else
		{
			dest[i] = 0;
			src_over = 1;
		}
		i++;
	}
	return (dest);
}
