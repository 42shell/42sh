/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_exec_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/11 13:51:04 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void	cache_add(char *command, char *path)
{
	if (!command || !path)
		return ;
	if (!g_binaries)
		g_binaries = ht_new(256, &free);
	ht_put(g_binaries, command, path);
}

static char	*cache_search(char *command)
{
	char	*ret;

	if (!command)
		return (NULL);
	if (!g_binaries
	|| !(ret = (char *)ht_get(g_binaries, command)))
		return (NULL);
	return (ret);
}

char		*get_exec_path(char *command, t_env *env)
{
	char		**path;
	char		*ret;
	int			i;
	struct stat b;

	i = 0;
	path = NULL;
	if ((ret = cache_search(command)))
		return (ft_strdup(ret));
	else if (ft_strchr(command, '/')
	&& (stat(command, &b) == 0 && (S_IXUSR & b.st_mode) && !S_ISDIR(b.st_mode)))
		ret = ft_strdup(command);
	else if ((path = split_path(get_env_var("PATH", env))))
	{
		while (path && path[i] && (ret = append_filename(path[i++], command)))
		{
			if (stat(ret, &b) == 0 && (S_IXUSR & b.st_mode) && !S_ISDIR(b.st_mode))
				break ;
			ft_memdel((void **)&ret);
		}
	}
	if (ret)
		cache_add(command, ft_strdup(ret));
	free_arr(path);
	return (ret);
}
