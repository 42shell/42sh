/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/18 14:43:58 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/06 15:24:25 by fratajcz         ###   ########.fr       */
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

t_var	*make_new_var(const char *name, const char *value, const int attributes,
		t_var_value_func value_func)
{
	t_var	*new_var;

	new_var = ft_xmalloc(sizeof(t_var));
	new_var->name = ft_strdup(name);
	if (value == NULL)
		new_var->value = ft_strdup("");
	else
		new_var->value = ft_strdup(value);
	new_var->exportstr = NULL;
	new_var->attributes = attributes;
	new_var->value_func = value_func;
	return (new_var);
}

/*
** See list of attributes in shell.h
*/

void	add_var(const char *name, const char *value, const int attributes)
{
	t_var	*new_var;

	new_var = make_new_var(name, value, attributes, NULL);
	ht_put(g_shell.vars, name, new_var);
}
