/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/22 00:10:15 by nbousset          #+#    #+#             */
/*   Updated: 2019/05/10 15:07:01 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char *str)
{
	char	*trim;
	char	*start;
	char	*end;
	size_t	len;

	if (!str)
		return (NULL);
	start = str;
	end = str + ft_strlen(str);
	while (ft_iswhitespace(*start))
		start++;
	while (start < (end - 1) && ft_iswhitespace(*(end - 1)))
		end--;
	len = end - start;
	if (!(trim = (char *)ft_xmalloc(len + 1)))
		return (NULL);
	ft_memcpy(trim, start, len);
	return (trim);
}
