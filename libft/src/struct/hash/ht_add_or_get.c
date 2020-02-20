/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ht_add_or_get.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/17 19:33:41 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/20 19:36:06 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#define FNV_OFFSET 2166136261
#define FNV_PRIME 16777619

/*
** FNV-1 hashing algorithm (same as bash)
*/

size_t			hash_string(const char *s)
{
	size_t i;

	i = FNV_OFFSET;
	while (*s)
	{
		i *= FNV_PRIME;
		i ^= *s++;
	}
	return (i);
}

/*
** Returns a pair from the bucket that matches key or NULL if no pair matches.
*/

t_pair			*get_pair(t_pair *bucket, const char *key)
{
	t_pair	*pair;

	if (bucket == NULL)
		return (NULL);
	pair = bucket;
	while (pair)
	{
		if (pair->key != NULL)
			if (ft_strcmp(pair->key, key) == 0)
				return (pair);
		pair = pair->next;
	}
	return (NULL);
}

/*
** Returns the value associated with key, or null if no key matches.
*/

void			*ht_get(const t_ht *map, const char *key)
{
	size_t		index;
	t_pair		*bucket;
	t_pair		*pair;

	index = hash_string(key) % map->size;
	bucket = &(map->buckets[index]);
	pair = get_pair(bucket, key);
	if (pair == NULL)
		return (NULL);
	return (pair->value);
}

/*
** Returns true if key is in map.
*/

bool			ht_exists(const t_ht *map, const char *key)
{
	size_t		index;
	t_pair		*bucket;
	t_pair		*pair;

	index = hash_string(key) % map->size;
	bucket = &(map->buckets[index]);
	pair = get_pair(bucket, key);
	if (pair == NULL)
		return (false);
	return (true);
}

/*
** Add a new key=value pair to the hash table.
*/

void			ht_put(t_ht *map, const char *key, void *value)
{
	t_pair		*bkt;
	t_pair		*pair;

	bkt = &(map->buckets[hash_string(key) % map->size]);
	if ((pair = get_pair(bkt, key)) != NULL)
	{
		map->free_value(pair->value);
		pair->value = value;
		return ;
	}
	if (bkt->key != NULL)
	{
		bkt->next = ft_xmalloc(sizeof(t_pair));
		*(bkt->next) = *(bkt);
		bkt->next->next = NULL;
	}
	bkt->key = ft_strdup(key);
	bkt->value = value;
}
