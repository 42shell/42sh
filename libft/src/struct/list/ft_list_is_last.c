/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_is_last.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 17:11:09 by fratajcz          #+#    #+#             */
/*   Updated: 2019/11/29 00:15:24 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** return 1 if list is the last entry in head
*/

int		ft_list_is_last(const t_list_head *list, const t_list_head *head)
{
	return (list->next == head);
}
