/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_del.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 17:11:09 by fratajcz          #+#    #+#             */
/*   Updated: 2019/11/29 00:15:24 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** deletes entry from a list
*/

void	ft_list_del(t_list_head *head)
{
	if (!head)
		return ;
	head->prev->next = head->next;
	head->next->prev = head->prev;
	free(head);
}
