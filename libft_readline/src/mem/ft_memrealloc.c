/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memrealloc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/10 00:37:27 by nbousset          #+#    #+#             */
/*   Updated: 2019/05/10 00:37:49 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memrealloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new;

	if (new_size < old_size)
		old_size = new_size;
	if (!(new = ft_memalloc(new_size)))
		return (NULL);
	if (ptr)
		ft_memcpy(new, ptr, old_size);
	return (new);
}
