/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dstr_add.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 23:03:42 by fratajcz          #+#    #+#             */
/*   Updated: 2019/11/21 23:05:58 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/libft.h"

int		ft_dstr_add(t_dstr *dstr, char c)
{
	char	*tmp;
	size_t	new_len;

	if (!dstr)
		return (0);
	new_len = dstr->len + 1;
	if (new_len > dstr->size - 1)
	{
		tmp = dstr->str;
		dstr->size *= 2;
		dstr->str = (char *)ft_memrealloc(dstr->str, dstr->len, dstr->size);
		free(tmp);
	}
	dstr->str[dstr->len] = c;
	dstr->str[++dstr->len] = 0;
	return (1);
}
