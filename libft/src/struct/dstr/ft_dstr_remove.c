/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dstr_remove.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 23:03:42 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/08 16:12:06 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_dstr_remove(t_dstr *dstr, size_t i, size_t len)
{
	char	*ptr;

	if (!dstr)
		return (0);
	ptr = dstr->str + i + len;
	ft_memmove(ptr - len, ptr, dstr->len - i - len + 1);
	dstr->len -= len;
	ft_bzero(dstr->str + dstr->len, len);
	return (len);
}
