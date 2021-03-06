/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/14 16:41:57 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/05 16:48:52 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_array		*array_new(size_t alloc)
{
	t_array *ret;

	ret = ft_xmalloc(sizeof(t_array));
	if (alloc == 0)
		ret->allocated = 4;
	else
		ret->allocated = alloc;
	ret->array = ft_xmalloc((ret->allocated + 1) * sizeof(void *));
	ret->size = 0;
	return (ret);
}

/*
** Copied from python list code
** the allocated growth pattern is 0, 4, 8, 16, 25, 35, 46, 58, 72, 88, …
*/

void		array_realloc(t_array *array)
{
	size_t	new_size;
	void	**new_array;
	size_t	i;

	new_size = array->size + 1;
	array->allocated = (new_size >> 3) + (new_size < 9 ? 3 : 6);
	array->allocated += new_size;
	new_array = ft_xmalloc((array->allocated + 1) * sizeof(void *));
	i = 0;
	while (i < array->size)
	{
		new_array[i] = array->array[i];
		i++;
	}
	free(array->array);
	array->array = new_array;
}

void		array_append(t_array *array, void *data)
{
	if (array->size == array->allocated)
		array_realloc(array);
	array->array[array->size++] = data;
}

/*
** Note that the popped value is NOT freed
*/

void		*array_pop(t_array *array)
{
	void	*ret;

	if (array->size == 0)
		return (NULL);
	ret = array->array[--array->size];
	array->array[array->size] = NULL;
	return (ret);
}

void		array_destroy(t_array *array)
{
	size_t i;

	if (array == NULL)
		return ;
	i = 0;
	while (i < array->size)
		free(array->array[i++]);
	free(array->array);
	free(array);
}
