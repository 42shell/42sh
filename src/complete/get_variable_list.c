/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_variable_list.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/11 14:47:35 by fratajcz          #+#    #+#             */
/*   Updated: 2020/09/11 14:47:35 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void			add_var_name_to_arr(const char *key, void *value, void *obj)
{
	t_array			*array;
	t_var			*var;

	var = value;
	if (var->attributes & V_HIDDEN)
		return ;
	array = obj;
	array_append(array, (char *)key);
}

t_list_head			*get_variable_list(char *partial)
{
	bool		brackets;
	t_list_head	*list;
	t_array		*vars;
	size_t		i;

	brackets = (partial[1] == '{');
	list = ft_list_first_head("");
	vars = array_new(ht_get_count(g_shell.vars));
	ht_enum(g_shell.vars, add_var_name_to_arr, vars);
	i = -1;
	while (++i < vars->size)
	{
		if (ft_strstr(vars->array[i], partial + 1 + brackets) == vars->array[i])
		{
			if (brackets)
				ft_list_add_tail(ft_strjoin_triple("${", vars->array[i], "}")
								, list);
			else
				ft_list_add_tail(ft_strjoin("$", vars->array[i]), list);
			(*g_comp_list_count)++;
		}
	}
	free(vars->array);
	free(vars);
	return (list);
}
