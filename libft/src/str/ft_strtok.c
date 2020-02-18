/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/16 15:14:02 by nbousset          #+#    #+#             */
/*   Updated: 2019/05/16 15:14:04 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtok(char *str, const char *sep)
{
	static char	*s;
	char		*w;

	if (!s || str != NULL)
		s = str;
	if (!sep || !(*sep))
		return (s);
	while (s && *s && ft_strchr(sep, *s))
		s++;
	w = s;
	while (s && *s)
	{
		if (ft_strchr(sep, *s))
		{
			while (ft_strchr(sep, *s))
			{
				*s = '\0';
				s++;
			}
			return (w);
		}
		s++;
	}
	s = NULL;
	return ((w && *w) ? w : NULL);
}
