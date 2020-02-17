/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ht_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/17 18:46:04 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/17 19:40:14 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_ht		*ht_new(size_t size)
{
	t_ht *map;

	map = ft_xmalloc(sizeof(t_ht));
	map->size = ft_next_power_of_two(size);
	map->buckets = ft_xmalloc(map->size * sizeof(t_bucket));
	return (map);
}

void		ht_delete(t_ht *map)
{
	size_t		i;
	size_t		j;
	t_bucket	*bucket;
	t_pair		*pair;

	if (map == NULL)
		return ;
	bucket = map->buckets;
	i = 0;
	while (i < map->size)
	{
		pair = bucket->pairs;
		j = 0;
		while (j < bucket->size)
		{
			free(pair->key);
			free(pair->value);
			pair++;
			j++;
		}
		free(bucket++->pairs);
		i++;
	}
	free(map->buckets);
	free(map);
}

/*
** Returns the number of keys in map.
*/

int			ht_get_count(const t_ht *map)
{
	size_t		i;
	size_t		j;
	size_t		count;
	t_bucket	*bucket;
	t_pair		*pair;

	bucket = map->buckets;
	i = 0;
	count = 0;
	while (i < map->size)
	{
		pair = bucket->pairs;
		j = 0;
		while (j < bucket->size)
		{
			count++;
			pair++;
			j++;
		}
		bucket++;
		i++;
	}
	return (count);
}

/*
** Execute enum_func(key, value, obj) for each key=value pair in the hash table.
*/

int			ht_enum(const t_ht *map, t_ht_enum_func enum_func, const void *obj)
{
	size_t		i;
	size_t		j;
	t_bucket	*bucket;
	t_pair		*pair;

	bucket = map->buckets;
	i = 0;
	while (i < map->size)
	{
		pair = bucket->pairs;
		j = 0;
		while (j < bucket->size)
		{
			enum_func(pair->key, pair->value, obj);
			pair++;
			j++;
		}
		bucket++;
		i++;
	}
	return (1);
}
