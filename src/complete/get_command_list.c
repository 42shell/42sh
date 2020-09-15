/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/16 15:01:14 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/05 17:15:47 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void			add_exec_matches_from_dir(t_list_head *comp_list,
											char *dirname, char *partial)
{
	DIR				*dirp;
	struct dirent	*dp;
	char			*cmd_path;
	struct stat		b;

	if ((dirp = opendir(dirname)))
	{
		while ((dp = readdir(dirp)))
		{
			cmd_path = append_filename(dirname, dp->d_name);
			if (ft_strstr(dp->d_name, partial) == dp->d_name
			&& stat(cmd_path, &b) == 0 && S_IXUSR & b.st_mode
			&& !S_ISDIR(b.st_mode))
			{
				ft_list_add_tail(ft_strdup(dp->d_name), comp_list);
				(*g_comp_list_count)++;
			}
			free(cmd_path);
		}
		closedir(dirp);
	}
}

static void			add_builtin_to_list(const char *key, void *value, void *obj)
{
	t_list_head *list;
	char		*partial;

	(void)value;
	list = ((void **)(obj))[0];
	partial = ((void **)(obj))[1];
	if (ft_strstr(key, partial) == key)
	{
		ft_list_add_tail(ft_strdup(key), list);
		(*g_comp_list_count)++;
	}
}

static t_list_head	*get_builtin_list(char *partial)
{
	t_list_head *builtin_list;
	void		*obj[2];

	builtin_list = ft_list_first_head("");
	obj[0] = builtin_list;
	obj[1] = partial;
	ht_enum(g_builtins, add_builtin_to_list, obj);
	return (builtin_list);
}

static t_list_head	*add_builtins_or_dirs(t_list_head *cmds, char *partial)
{
	t_list_head		*builtins;

	builtins = get_builtin_list(partial);
	ft_list_splice(cmds, builtins);
	free(builtins);
	if (ft_list_empty(cmds))
	{
		free(cmds);
		return (get_file_list(partial, DIRONLY));
	}
	return (cmds);
}

t_list_head			*comp_get_command_list(char *partial)
{
	t_list_head		*comp_list;
	char			**path_dirs;
	char			*path;
	int				i;

	path = get_var_value("PATH");
	if (path == NULL)
		return (NULL);
	path_dirs = split_path(path);
	comp_list = ft_list_first_head("");
	i = 0;
	while (path_dirs[i])
	{
		add_exec_matches_from_dir(comp_list, path_dirs[i], partial);
		i++;
	}
	free_arr(path_dirs);
	return (add_builtins_or_dirs(comp_list, partial));
}
