/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/08 18:37:13 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 18:37:14 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char	*get_var_value(const char *name)
{
	t_var *var;

	if ((var = ht_get(g_shell.vars, name)) == NULL)
		return (NULL);
	if (var->attributes & V_SPECIAL)
		return (var->value_func());
	return (var->value);
}

void	set_var_attributes(const char *name, const int attributes)
{
	t_var *var;

	var = ht_get(g_shell.vars, name);
	if (var == NULL)
		return ;
	var->attributes |= attributes;
}

/*
** attributes are not overwritten but added to the var if it already exists
*/

void	set_var(const char *name, const char *value, const int attributes)
{
	t_var *var;

	var = ht_get(g_shell.vars, name);
	if (var == NULL)
	{
		add_var(name, value, attributes);
		return ;
	}
	free(var->value);
	if (value == NULL)
		var->value = ft_strdup("");
	else
		var->value = ft_strdup(value);
	free(var->exportstr);
	var->exportstr = NULL;
	var->attributes |= attributes;
}

void	unset_var(const char *name)
{
	ht_remove(g_shell.vars, name);
}

bool	var_exists(const char *name)
{
	return (ht_get(g_shell.vars, name) != NULL);
}
