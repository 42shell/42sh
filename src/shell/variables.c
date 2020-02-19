/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/18 14:43:58 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/19 14:41:37 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** value_func can be NULL
*/

void	free_var(void *var_ptr)
{
	t_var *var;

	var = var_ptr;
	free(var->name);
	free(var->value);
	free(var);
}

t_var	*make_new_var(char *name, char *value, int attributes,
		t_var_value_func value_func)
{
	t_var	*new_var;

	new_var = ft_xmalloc(sizeof(t_var));
	new_var->name = ft_strdup(name);
	new_var->value = ft_strdup(value);
	new_var->exportstr = NULL;
	new_var->attributes = attributes;
	new_var->value_func = value_func;
	return (new_var);
}

/*
** See list of attributes in shell.h
*/

void	add_var(t_ht *map, char *name, char *value, int attributes)
{
	t_var	*new_var;

	new_var = make_new_var(name, value, attributes, NULL);
	ht_put(map, name, new_var);
}

/*
** Adds all variables in env to map
*/

void	import_env(t_ht *map, char **env)
{
	int		i;
	char	*value;

	i = 0;
	while (env[i])
	{
		value = ft_strchr(env[i], '=') + 1;
		value[-1] = '\0';
		add_var(map, env[i], value, V_EXPORT);
		value[-1] = '=';
		i++;
	}
}
