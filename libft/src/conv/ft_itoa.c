/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 16:52:18 by nbousset          #+#    #+#             */
/*   Updated: 2020/05/06 18:08:36 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strrev(char *str)
{
	int		i;
	int		j;
	char	tmp;

	i = 0;
	j = ft_strlen(str) - 1;
	while (i < j)
	{
		tmp = str[i];
		str[i] = str[j];
		str[j] = tmp;
		i++;
		j--;
	}
	return (str);
}

char		*ft_itoa(int n, char *str)
{
	int				i;
	unsigned int	n_abs;

	n_abs = n < 0 ? -n : n;
	i = 0;
	if (n_abs == 0)
		str[i++] = '0';
	while (n_abs != 0)
	{
		str[i] = n_abs % 10 + 48;
		n_abs /= 10;
		i++;
	}
	if (n < 0)
	{
		str[i] = '-';
		i++;
	}
	str[i] = '\0';
	return (ft_strrev(str));
}
