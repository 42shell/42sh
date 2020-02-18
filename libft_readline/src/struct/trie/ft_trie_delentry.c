/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_trie_delentry.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:18:07 by fratajcz          #+#    #+#             */
/*   Updated: 2019/12/08 20:07:15 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_trie	*ft_trie_delentry(t_trie *trie, unsigned char *word)
{
	t_trie	*parent;
	t_trie	*entry;
	int		index;

	if (!(entry = ft_trie_getentry(trie, word)))
		return (NULL);
	entry->isword = false;
	entry->data = NULL;
	if (entry->nodes_count == 0)
	{
		while (entry->parent->nodes_count == 1
		&& !entry->parent->isword)
			entry = entry->parent;
		index = ft_trie_get_index(entry->parent, entry->character);
		entry->parent->nodes[index--] = NULL;
		while (++index < entry->parent->nodes_count)
			entry->parent->nodes[index] = entry->parent->nodes[index + 1];
		entry->parent->nodes_count--;
		parent = entry->parent;
		ft_trie_del(&entry);
		return (parent);
	}
	return (NULL);
}
