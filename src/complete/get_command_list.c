/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/16 15:01:14 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/11 13:42:45 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void	add_exec_matches_from_dir(t_list_head *comp_list, char *dirname,
			char *partial)
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

t_list_head	*comp_get_command_list(char *partial)
{
	t_list_head		*comp_list;
	char			**path_dirs;
	char			*path;
	int				i;

	path = get_env_var("PATH", g_env);
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
	return (comp_list);
}
