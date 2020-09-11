/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_splice.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/12 00:03:14 by fratajcz          #+#    #+#             */
/*   Updated: 2020/09/12 00:03:14 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** add list2 at the end of list1
** you need to free the head of list2 yourself
*/

void	ft_list_splice(t_list_head *list1, t_list_head *list2)
{
	if (!ft_list_empty(list2))
	{
		list1->prev->next = list2->next;
		list2->next->prev = list1->prev;
		list2->prev->next = list1;
		list1->prev = list2->prev;
	}
}
