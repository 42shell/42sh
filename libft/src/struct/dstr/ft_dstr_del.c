/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dstr_del.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 23:03:42 by fratajcz          #+#    #+#             */
/*   Updated: 2019/11/21 23:05:58 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_dstr_del(void **ptr, void *priv)
{
	t_dstr	**dstr;

	(void)priv;
	if (!ptr || !*ptr)
		return ;
	dstr = (t_dstr **)ptr;
	ft_memdel((void **)&(*dstr)->str);
	ft_memdel((void **)dstr);
}
