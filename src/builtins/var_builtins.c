/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 15:13:13 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/19 15:36:02 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		builtin_export(char **argv)
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
			set_env_var(argv[i], value, g_env);
		i++;
	}
	if (i == 1)
		ft_dprintf(2, "usage: export [name[=value]]\n");
	return (ret);
}
