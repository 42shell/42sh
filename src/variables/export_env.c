/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz </var/spool/mail/fratajcz>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 15:36:55 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/06 16:48:46 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** obj is the t_array to which we add vars if they are flagged for export
*/

static void	enum_add_var_to_env(const char *key, void *value, void *obj)
{
	t_array *env;
	t_var	*var;

	(void)key;
	env = obj;
	var = value;
	if (var->attributes & V_EXPORT)
	{
		if (var->exportstr == NULL)
			var->exportstr = ft_strjoin_triple((char *)key, "=", var->value);
		array_append(env, ft_strdup(var->exportstr));
	}
}

t_array		*export_env(t_ht *map)
{
	t_array *env;

	env = array_new(16);
	ht_enum(map, enum_add_var_to_env, env);
	return (env);
}

/*
** Adds all variables in env to map
*/

void		import_env(char **env)
{
	int		i;
	char	*value;

	i = 0;
	while (env[i])
	{
		value = ft_strchr(env[i], '=') + 1;
		value[-1] = '\0';
		set_var(env[i], value, V_EXPORT);
		value[-1] = '=';
		i++;
	}
}
