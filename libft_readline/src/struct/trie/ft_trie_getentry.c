/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_trie_getentry.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:18:07 by fratajcz          #+#    #+#             */
/*   Updated: 2019/12/08 20:07:15 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_trie	*ft_trie_getentry(t_trie *root, unsigned char *word)
{
	int		i;
	int		index;

	i = 0;
	while (word[i])
	{
		index = ft_trie_get_index(root, word[i]);
		if (!root->nodes || !root->nodes[index]
		|| root->nodes[index]->character != word[i])
			return (NULL);
		root = root->nodes[index];
		i++;
	}
	return (root->isword ? root : NULL);
}
