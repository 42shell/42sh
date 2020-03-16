/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/14 17:52:18 by fratajcz         ###   ########.fr       */
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

void		argv_del(t_argv **argv)
{
		free_arr((*argv)->argv);
		free((*argv)->path);
		ft_memdel((void **)argv);
}

static char	*get_executable_path(char *command)
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
	else if ((path = split_path(get_env_var("PATH"))))
	{
		while (path && path[i] && (ret = append_filename(path[i++], command)))
		{
			if (stat(ret, &b) == 0 && (S_IXUSR & b.st_mode) && !S_ISDIR(b.st_mode))
				break ;
			ft_memdel((void **)&ret);
		}
	}
	if (ret)
		cache_add(ft_strdup(command), ft_strdup(ret));
	free_arr(path);
	return (ret);
}

static char	**get_argv_array(t_command *command)
{
	char	**argv;
	int		argc;
	int		i;

	i = 0;
	argc = 0;
	if (!command->argv)
		return (NULL);
	while (command->argv[argc])
		argc++;
	argv = ft_xmalloc(sizeof(char *) * (argc + 1));
	while (i < argc)
	{
		argv[i] = ft_strdup(command->argv[i]->value->str);
		i++;
	}
	return (argv);
}

t_argv		*get_argv(t_command *command)
{
	t_argv	*argv;

	argv = ft_xmalloc(sizeof(t_argv));
	argv->argv = get_argv_array(command);
	if (!(argv->path = get_executable_path(argv->argv[0])))
		argv_del(&argv);
	return (argv);
}
