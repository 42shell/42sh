/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_trie_addentry.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:18:07 by fratajcz          #+#    #+#             */
/*   Updated: 2019/12/08 20:07:15 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_trie_addentry(t_trie *root, unsigned char *word, void *data,
																	long flags)
{
	int		index;
	int		i;

	i = 0;
	while (word[i])
	{
		index = ft_trie_get_index(root, word[i]);
		if (!root->nodes || !root->nodes[index]
		|| root->nodes[index]->character != word[i])
			ft_trie_addnode(root, ft_trie_new(word[i], NULL, false, 0));
		root = root->nodes[index];
		i++;
	}
	root->data = data;
	root->flags = flags;
	root->isword = true;
}
