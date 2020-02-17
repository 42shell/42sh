/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_matches.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 19:21:46 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/17 13:29:53 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	swap(char **s1, char **s2)
{
	char *tmp;

	tmp = *s1;
	*s1 = *s2;
	*s2 = tmp;
}

static int	partition(char **array, int lower, int upper)
{
	char	*pivot;
	int		i;
	int		j;

	pivot = array[upper];
	i = lower;
	j = lower;
	while (j < upper)
	{
		if (ft_strcmp(array[j], pivot) < 0)
		{
			swap(&array[i], &array[j]);
			i++;
		}
		j++;
	}
	swap(&array[i], &array[upper]);
	return (i);
}

static void	quicksort(char **array, int lower, int upper)
{
	int	p;

	if (lower < upper)
	{
		p = partition(array, lower, upper);
		quicksort(array, lower, p - 1);
		quicksort(array, p + 1, upper);
	}
}

void		sort_matches(char **array, int size)
{
	quicksort(array, 0, size - 1);
}
