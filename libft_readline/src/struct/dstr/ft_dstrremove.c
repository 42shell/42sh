/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dstrremove.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 23:03:42 by fratajcz          #+#    #+#             */
/*   Updated: 2019/11/21 23:05:58 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_dstrremove(t_dstr *dstr, size_t i, size_t len)
{
	char	*ptr;

	if (!dstr)
		return (0);
	ptr = dstr->str + i + len;
	ft_memmove(ptr - len, ptr, ft_strlen(ptr) + 1);
	dstr->len -= len;
	return (len);
}
