/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ht_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/17 18:46:04 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/20 20:28:01 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** free_value is called when we need to free a key's value (for example if we
** need to free elements of a struct stored in value)
*/

t_ht		*ht_new(size_t size, t_ht_free_func free_value)
{
	t_ht *map;

	map = ft_xmalloc(sizeof(t_ht));
	map->size = ft_next_power_of_two(size);
	map->buckets = ft_xmalloc(map->size * sizeof(t_pair));
	map->free_value = free_value;
	return (map);
}

void		ht_delete(t_ht *map)
{
	size_t		i;
	t_pair		*pair;
	t_pair		*tmp;
	t_pair		*buckets;

	if (map == NULL)
		return ;
	i = 0;
	buckets = map->buckets;
	while (i < map->size)
	{
		pair = buckets;
		while (pair)
		{
			free(pair->key);
			map->free_value(pair->value);
			tmp = pair;
			pair = pair->next;
			free(tmp);
		}
		buckets++;
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
	size_t		count;
	t_pair		*bucket;
	t_pair		*pair;

	bucket = map->buckets;
	i = 0;
	count = 0;
	while (i < map->size)
	{
		pair = bucket;
		while (pair && pair->key)
		{
			count++;
			pair = pair->next;
		}
		bucket++;
		i++;
	}
	return (count);
}

/*
** Execute enum_func(key, value, obj) for each key=value pair in the hash table.
*/

int			ht_enum(const t_ht *map, t_ht_enum_func enum_func, void *obj)
{
	size_t		i;
	t_pair		*bucket;
	t_pair		*pair;

	bucket = map->buckets;
	i = 0;
	while (i < map->size)
	{
		pair = bucket;
		while (pair && pair->key)
		{
			enum_func(pair->key, pair->value, obj);
			pair = pair->next;
		}
		bucket++;
		i++;
	}
	return (1);
}

/*
** Remove a pair from a hash table
** if there is a pair after the one we want to delete, we copy the contents of
** the next pair into the pair we want to delete and we free the next pair
*/

void		ht_remove(const t_ht *map, const char *key)
{
	t_pair *prev;
	t_pair *pair;

	pair = &(map->buckets[hash_string(key) % map->size]);
	prev = NULL;
	while (pair)
	{
		if (pair->key != NULL && ft_strcmp(pair->key, key) == 0)
		{
			ft_memdel((void **)&pair->key);
			map->free_value(pair->value);
			if (prev != NULL)
				prev->next = pair->next;
			if (prev != NULL)
				return (free(pair));
			else if (pair->next != NULL)
			{
				pair->value = pair->next->value;
				pair->key = pair->next->key;
				return (ft_memdel((void **)&pair->next));
			}
		}
		prev = pair;
		pair = pair->next;
	}
}
