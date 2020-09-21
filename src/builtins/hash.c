/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 10:40:14 by fratajcz          #+#    #+#             */
/*   Updated: 2020/09/20 10:40:14 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void	display_binary(const char *key, void *value, void *obj)
{
	(void)obj;
	ft_printf("%s=%s\n", key, (char *)value);
}

int			builtin_hash(char *argv[], t_array *env)
{
	int		i;
	int		ret;
	char	*path;

	i = 0;
	ret = 0;
	if (ft_strequ(argv[1], "-r"))
	{
		ht_delete(g_binaries);
		g_binaries = ht_new(256, free);
		i = 1;
	}
	while (argv[++i])
	{
		path = get_exec_path(argv[i], env);
		if (path == NULL)
		{
			ft_dprintf(2, "42sh: hash: %s: not found\n", argv[i]);
			ret = 1;
		}
		free(path);
	}
	if (!argv[1])
		ht_enum(g_binaries, display_binary, NULL);
	return (ret);
}
