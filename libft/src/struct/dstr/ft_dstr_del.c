/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dstr_del.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 23:03:42 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/02 01:00:46 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_dstr_del(t_dstr **ptr)
{
	t_dstr	**dstr;

	if (!ptr || !*ptr)
		return ;
	dstr = (t_dstr **)ptr;
	ft_memdel((void **)&(*dstr)->str);
	ft_memdel((void **)dstr);
}
