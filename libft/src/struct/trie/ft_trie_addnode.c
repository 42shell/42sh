/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_trie_addnode.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:18:07 by fratajcz          #+#    #+#             */
/*   Updated: 2019/12/08 20:07:15 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_trie	**realloc_nodes(t_trie *trie, size_t new_size)
{
	t_trie	**new_nodes;
	int		i;

	i = -1;
	if (!(new_nodes = (t_trie **)ft_memalloc(sizeof(t_trie *)
														* (new_size + 1))))
		return (NULL);
	if (trie->nodes)
	{
		while (trie->nodes[++i])
			new_nodes[i] = trie->nodes[i];
		free(trie->nodes);
	}
	trie->nodes_size = new_size;
	trie->nodes = new_nodes;
	return (new_nodes);
}

void			ft_trie_addnode(t_trie *root, t_trie *node)
{
	size_t	new_size;
	int		index;
	int		i;

	if (root->nodes_size - root->nodes_count < 1)
	{
		new_size = root->nodes_size > 16 ?
		root->nodes_size + 8 : ft_next_power_of_two(root->nodes_size + 1);
		realloc_nodes(root, new_size);
	}
	index = ft_trie_get_index(root, node->character);
	i = root->nodes_count + 1;
	while (--i > index)
		root->nodes[i] = root->nodes[i - 1];
	root->nodes[index] = node;
	root->nodes_count++;
	node->parent = root;
}
