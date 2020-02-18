/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_first_head.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 17:11:09 by fratajcz          #+#    #+#             */
/*   Updated: 2019/11/29 00:15:24 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list_head	*ft_list_first_head(void *data)
{
	t_list_head *new;

	if ((new = (t_list_head *)ft_xmalloc(sizeof(t_list_head))) == NULL)
		return (NULL);
	new->data = data;
	new->prev = new;
	new->next = new;
	return (new);
}
