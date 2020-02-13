/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_matches.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 19:21:46 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/13 20:17:45 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_strcasecmp(const char *s1, const char *s2)
{
	const unsigned char	*p1;
	const unsigned char	*p2;
	char				c1;
	char				c2;

	p1 = (const unsigned char *) s1;
	p2 = (const unsigned char *) s2;
	if (p1 == p2)
		return (0);
	while (*p1 && *p1 == *p2)
	{
		p1++;
		p2++;
	}
	c1 = *p1;
	c2 = *p2;
	c1 = ft_isupper(c1) ? c1 + ('a' - 'A') : c1;
	c2 = ft_isupper(c2) ? c2 + ('a' - 'A') : c2;
	return (c1 - c2);
}

static void	swap(void **s1, void **s2)
{
	void **tmp;

	tmp = *s1;
	*s1 = *s2;
	*s2 = tmp;
}

static int	partition(t_node **array, int lower, int upper)
{
	char	*pivot;
	int		i;
	int		j;

	pivot = array[upper]->data;
	i = lower;
	j = lower;
	while (j < upper)
	{
		if (ft_strcasecmp(array[j]->data, pivot) < 0)
		{
			swap(&array[i]->data, &array[j]->data);
			i++;
		}
		j++;
	}
	swap(&array[i]->data, &array[upper]->data);
	return (i);
}

static void	quicksort(t_node **array, int lower, int upper)
{
	int	p;

	if (lower < upper)
	{
		p = partition(array, lower, upper);
		quicksort(array, lower, p - 1);
		quicksort(array, p + 1, upper);
	}
}

void		sort_matches(t_node **array, int size)
{
	quicksort(array, 0, size - 1);
}
