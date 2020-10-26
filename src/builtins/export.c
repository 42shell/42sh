/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 15:13:13 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 17:28:16 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void	export_one_var(char *name, char *value)
{
	if (value == NULL && var_exists(name))
		set_var_attributes(name, V_EXPORT);
	else if (value)
		set_var(name, value, V_EXPORT);
}

char		*get_quoted_string(const char *str)
{
	t_dstr	*quoted;
	char	*ret;

	quoted = ft_dstr_new(32);
	ft_dstr_add(quoted, '"');
	while (*str)
	{
		if (*str == '"' || *str == '\\' || *str == '$' || *str == '`')
			ft_dstr_add(quoted, '\\');
		ft_dstr_add(quoted, *str);
		str++;
	}
	ft_dstr_add(quoted, '"');
	ret = quoted->str;
	free(quoted);
	return (ret);
}

static void	add_var_to_arr(const char *key, void *value, void *obj)
{
	t_array			*array;
	t_var			*var;
	char			*quoted;

	var = value;
	if (var->attributes & V_HIDDEN || !(var->attributes & V_EXPORT))
		return ;
	array = obj;
	quoted = get_quoted_string(var->value);
	array_append(array, ft_strjoin_triple(key, "=", quoted));
	free(quoted);
}

int			export_p(void)
{
	t_array *vars;
	size_t	i;

	vars = array_new(32);
	ht_enum(g_shell.vars, add_var_to_arr, vars);
	sort_matches((char **)vars->array, vars->size);
	i = 0;
	while (i < vars->size)
		ft_printf("export %s\n", vars->array[i++]);
	array_destroy(vars);
	return (0);
}

int			builtin_export(char **argv, __attribute__((unused)) t_array *env)
{
	int		i;
	int		ret;
	char	*value;

	i = 0;
	ret = 0;
	if (argv[1] && ft_strequ(argv[1], "-p"))
		return (export_p());
	while (argv[++i])
	{
		value = NULL;
		if (argv[i][0] != '=' && (value = ft_strchr(argv[i], '=')))
			*(value++) = '\0';
		if (!is_valid_var_name(argv[i]))
		{
			ft_dprintf(2, "42sh: export: `%s': not a valid identifier\n",
					argv[i]);
			ret = 1;
		}
		else
			export_one_var(argv[i], value);
	}
	if (i == 1)
		ft_dprintf(2, "usage: export [name[=value]] or export -p\n");
	return (ret);
}
