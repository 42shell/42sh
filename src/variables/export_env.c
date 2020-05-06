/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz </var/spool/mail/fratajcz>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 15:36:55 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/06 15:24:34 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** obj is the t_env to which we add vars if they are flagged for export
*/

static void	enum_add_var_to_env(const char *key, void *value, void *obj)
{
	t_env *env;
	t_var *var;

	(void)key;
	env = obj;
	var = value;
	if (var->attributes & V_EXPORT)
		add_env_var(var->name, var->value, env);
}

t_env		*export_env(t_ht *map)
{
	t_env *env;

	env = ft_xmalloc(sizeof(t_env));
	env->env = ft_xmalloc(7 * sizeof(char *));
	env->capacity = 7;
	env->size = 1;
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
