/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dstr_append.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 17:26:19 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/13 17:27:10 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/libft.h"

int		ft_dstr_append(t_dstr *dstr, char *str)
{
	return (ft_dstr_insert(dstr, dstr->len, str, ft_strlen(str)));
}
