/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 18:50:27 by nbousset          #+#    #+#             */
/*   Updated: 2019/04/21 09:26:32 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*str;
	size_t	size;

	if (!s1)
		return (NULL);
	size = ft_strlen(s1);
	if (!(str = (char *)ft_xmalloc(size + 1)))
		return (NULL);
	ft_strcpy(str, s1);
	return (str);
}
