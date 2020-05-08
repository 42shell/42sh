/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_file_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/16 15:04:39 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 18:21:37 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static char			*get_dir_to_search(char *partial)
{
	int		i;

	i = ft_strlen(partial);
	while (i > 0 && partial[i] != '/')
		i--;
	if (i == 0 && partial[0] != '/')
		return (ft_strdup("."));
	if (i == 0 && partial[0] == '/')
		return (ft_strdup("/"));
	return (ft_strndup(partial, i));
}

static t_list_head	*get_matches_in_cwd(DIR *dirp, char *partial, int flags)
{
	struct dirent	*dp;
	t_list_head		*comp_list;

	comp_list = ft_list_first_head("");
	while ((dp = readdir(dirp)))
	{
		if (ft_strstr(dp->d_name, partial) == dp->d_name
				&& !ft_strequ(dp->d_name, ".") && !ft_strequ(dp->d_name, ".."))
		{
			if (is_dir(dp->d_name)
			&& ++(*g_comp_list_count))
				ft_list_add_tail(ft_strjoin(dp->d_name, "/"), comp_list);
			else if (!(flags & DIRONLY)
			&& (!(flags & EXECONLY) || is_exec(dp->d_name))
			&& ++(*g_comp_list_count))
				ft_list_add_tail(ft_strdup(dp->d_name), comp_list);
		}
	}
	return (comp_list);
}

static t_list_head	*get_matches_in_dir(DIR *dirp, char *dir, char *partial,
										int flags)
{
	struct dirent	*dp;
	char			*path;
	t_list_head		*comp_list;

	comp_list = ft_list_first_head("");
	while ((dp = readdir(dirp)))
	{
		path = append_filename(dir, dp->d_name);
		if (ft_strstr(path, partial) == path
		&& !ft_strequ(dp->d_name, ".") && !ft_strequ(dp->d_name, ".."))
		{
			if (is_dir(path)
			&& ++(*g_comp_list_count))
				ft_list_add_tail(ft_strjoin(path, "/"), comp_list);
			else if (!(flags & DIRONLY)
			&& (!(flags & EXECONLY) || is_exec(dp->d_name))
			&& ++(*g_comp_list_count))
				ft_list_add_tail(ft_strdup(path), comp_list);
		}
		free(path);
	}
	return (comp_list);
}

t_list_head			*get_file_list(char *partial, int flags)
{
	char			*dir;
	DIR				*dirp;
	t_list_head		*comp_list;

	dir = get_dir_to_search(partial);
	if ((dirp = opendir(dir)) == NULL)
	{
		free(dir);
		return (NULL);
	}
	if (ft_strequ(dir, ".") && (ft_strstr(partial, "./") != partial))
		comp_list = get_matches_in_cwd(dirp, partial, flags);
	else
		comp_list = get_matches_in_dir(dirp, dir, partial, flags);
	free(dir);
	closedir(dirp);
	return (comp_list);
}
