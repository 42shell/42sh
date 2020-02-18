/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:18:07 by fratajcz          #+#    #+#             */
/*   Updated: 2019/12/08 20:07:15 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/libft.h"

t_node		*node_new(void *data)
{
	t_node *node;

	node = (t_node *)ft_xmalloc(sizeof(t_node));
	if (node == NULL)
		return (NULL);
	node->data = data;
	node->nb_children = 0;
	node->capacity = 0;
	node->child = NULL;
	return (node);
}

static int	node_realloc(t_node *node)
{
	t_node	**child_new;
	int		i;

	child_new = (t_node **)ft_xmalloc(sizeof(t_node **) * node->capacity * 2);
	if (child_new == NULL)
		return (-1);
	node->capacity *= 2;
	i = 0;
	while (i < node->nb_children)
	{
		child_new[i] = node->child[i];
		i++;
	}
	free(node->child);
	node->child = child_new;
	return (0);
}

int			node_add_child(t_node *parent, t_node *child)
{
	if (parent->child == NULL)
	{
		parent->child = (t_node **)ft_xmalloc(sizeof(t_node *) * 2);
		if (parent->child == NULL)
			return (-1);
		parent->capacity = 2;
	}
	else if (parent->capacity == parent->nb_children)
	{
		if (node_realloc(parent) == -1)
			return (-1);
	}
	parent->child[parent->nb_children++] = child;
	return (0);
}
