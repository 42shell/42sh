/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_command_from_list.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/28 19:44:40 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			remove_command_from_stupid_compound_command(t_command **tree,
														t_command *command)
{
	if ((*tree)->type == GROUP)
		return (remove_command_from_list(&(*tree)->value.group->list,
										command));
	if ((*tree)->type == IF_CLAUSE)
	{
		return (remove_command_from_list(&(*tree)->value.if_clause->if_part,
										command)
		|| remove_command_from_list(&(*tree)->value.if_clause->then_part,
									command)
		|| remove_command_from_list(&(*tree)->value.if_clause->else_part,
									command));
	}
	if ((*tree)->type == WHILE_CLAUSE)
	{
		return (remove_command_from_list(&(*tree)->value.if_clause->if_part,
										command)
		|| remove_command_from_list(&(*tree)->value.if_clause->then_part,
									command));
	}
	return (0);
}

int			remove_command_from_stupid_tree(t_command **tree,
											t_command *command)
{
	if (*tree == command)
	{
		if ((*tree)->next)
			(*tree)->next->prev = (*tree)->prev;
		if ((*tree)->prev)
			(*tree)->prev->next = (*tree)->next;
		else if ((*tree = (*tree)->next))
			(*tree)->prev = NULL;
		*tree = NULL;
		return (1);
	}
	if ((*tree)->flags & CMD_COMPOUND)
		return (remove_command_from_stupid_compound_command(tree, command));
	if ((*tree)->type == CONNECTION)
	{
		if (!remove_command_from_stupid_tree(&(*tree)->value.connection->left,
											command))
			return (remove_command_from_stupid_tree(
								&(*tree)->value.connection->right, command));
		return (1);
	}
	return (0);
}

int			remove_command_from_list(t_command **head, t_command *command)
{
	t_command	*ptr;

	ptr = *head;
	while (ptr)
	{
		if (remove_command_from_stupid_tree(&ptr, command))
			return (1);
		ptr = ptr->next;
	}
	return (0);
}
