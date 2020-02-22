/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_trie_get_index.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:18:07 by fratajcz          #+#    #+#             */
/*   Updated: 2019/12/08 20:07:15 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_trie_get_index(t_trie *trie, unsigned char character)
{
	int		start;
	int		pivot;
	int		stop;

	start = 0;
	stop = trie->nodes_count;
	while (start != stop)
	{
		pivot = start + ((stop - start) / 2);
		if (character == trie->nodes[pivot]->character)
			return (pivot);
		else if (character > trie->nodes[pivot]->character)
			start = pivot + 1;
		else
			stop = pivot;
	}
	return (start);
}
