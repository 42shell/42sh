/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dstr_dup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 23:03:42 by fratajcz          #+#    #+#             */
/*   Updated: 2019/11/21 23:05:58 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_dstr	*ft_dstr_dup(t_dstr *dstr)
{
	t_dstr	*dup;

	if (!(dup = ft_dstr_new(dstr->size)))
		return (NULL);
	ft_memcpy(dup->str, dstr->str, dstr->len);
	dup->len = dstr->len;
	return (dup);
}
