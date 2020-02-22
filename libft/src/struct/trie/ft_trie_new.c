/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_trie_new.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:18:07 by fratajcz          #+#    #+#             */
/*   Updated: 2019/12/08 20:07:15 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_trie	*ft_trie_new(unsigned char character, void *data, bool isword,
																	long flags)
{
	t_trie		*new;

	if (!(new = (t_trie *)ft_memalloc(sizeof(t_trie))))
		return (NULL);
	new->character = character;
	new->data = data ? data : NULL;
	new->isword = isword;
	new->flags = flags;
	return (new);
}
