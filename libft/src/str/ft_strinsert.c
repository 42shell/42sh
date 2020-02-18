/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strinsert.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: exam <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 19:08:56 by exam              #+#    #+#             */
/*   Updated: 2019/06/21 19:36:29 by exam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_strinsert(char *str, char *insert, ssize_t len)
{
	if (!str)
		return ;
	if (len == -1)
		len = ft_strlen(insert);
	ft_memmove(str + len, str, ft_strlen(str) + 1);
	ft_memcpy(str, insert, len);
}
