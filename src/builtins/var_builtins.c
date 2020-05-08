/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 15:13:13 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/07 16:31:25 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		builtin_export(char **argv, __attribute__((unused)) t_array *env)
{
	int		i;
	int		ret;
	char	*value;

	i = 1;
	ret = 0;
	while (argv[i])
	{
		value = NULL;
		if ((value = ft_strchr(argv[i], '=')))
			*(value++) = '\0';
		if (!is_valid_var_name(argv[i]))
		{
			ft_dprintf(2, "42sh: export: '%s': not a valid identifier\n",
					argv[i]);
			ret = 1;
		}
		else 
		{
			if (value == NULL && var_exists(argv[i]))
				set_var_attributes(argv[i], V_EXPORT);
			else
				set_var(argv[i], value, V_EXPORT);
		}
		i++;
	}
	if (i == 1)
		ft_dprintf(2, "usage: export [name[=value]]\n");
	return (ret);
}

int		builtin_unset(char **argv, __attribute__((unused)) t_array *env)
{
	int		i;
	int		ret;

	i = 1;
	ret = 0;
	while (argv[i])
	{
		if (!is_valid_var_name(argv[i]))
		{
			ft_dprintf(2, "42sh: unset: '%s': not a valid identifier\n",
					argv[i]);
			ret = 1;
		}
		else
			unset_var(argv[i]);
		i++;
	}
	return (ret);
}

/*
** obj points to a t_array, we add key=value to this array for each variable
*/

static void	add_var_to_arr(const char *key, void *value, void *obj)
{
	t_array			*array;
	t_var			*var;

	var = value;
	if (var->attributes & V_HIDDEN)
		return ;
	array = obj;
	if (var->exportstr == NULL)
		var->exportstr = ft_strjoin_triple((char *)key, "=", var->value);
	array_append(array, var->exportstr);
}

int			builtin_set(char **argv, __attribute__((unused)) t_array *env)
{
	t_array *vars;
	size_t	i;

	(void)argv;
	vars = array_new(ht_get_count(g_shell.vars));
	ht_enum(g_shell.vars, add_var_to_arr, vars);
	sort_matches((char **)vars->array, vars->size);
	i = 0;
	while (i < vars->size)
		ft_printf("%s\n", vars->array[i++]);
	free(vars->array);
	free(vars);
	return (0);
}
