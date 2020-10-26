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
	else
		set_var(name, value, V_EXPORT);
}

int			builtin_export(char **argv, __attribute__((unused)) t_array *env)
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
			export_one_var(argv[i], value);
		i++;
	}
	if (i == 1)
		ft_dprintf(2, "usage: export [name[=value]]\n");
	return (ret);
}
