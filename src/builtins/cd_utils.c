/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 14:02:44 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/29 14:26:13 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

#define CD_P		1
#define PRINT_DIR	2

char	*get_pwd(t_env *env)
{
	static char	buf[PATH_MAX + 1];
	char		*pwd;

	pwd = get_env_var("PWD", env);
	if (pwd == NULL)
	{
		if (getcwd(buf, PATH_MAX + 1) == NULL)
		{
			ft_putstr_fd("cd: PWD not set\ngetcwd: could not get current dir\n"
					, 2);
			return (NULL);
		}
		return (buf);
	}
	return (pwd);
}

char	*get_home_dir(t_env *env)
{
	char *home;

	home = get_env_var("HOME", env);
	if (home == NULL)
		write(2, "42sh: cd: HOME not set\n", 23);
	return (ft_strdup(home));
}

/*
** This is step 7 in the posix cd algorithm.
*/

void	append_curpath_to_pwd(char **curpath, t_env *env)
{
	char *new;
	char *pwd;

	if ((pwd = get_pwd(env)))
	{
		new = append_filename(pwd, *curpath);
		free(*curpath);
		*curpath = new;
	}
	else
	{
		free(*curpath);
		*curpath = NULL;
	}
}

void	remove_dots(char *curpath)
{
	char *match;

	while ((match = ft_strstr(curpath, "/./")))
		ft_strcpy(match, match + 2);
	while ((match = ft_strstr(curpath, "./"))
			&& (match == curpath || *(match - 1) == '/'))
		ft_strcpy(match, match + 2);
	while ((match = ft_strstr(curpath, "/.")) && *(match + 2) == '\0')
		ft_strcpy(match, match + 2);
}

int		remove_dotdots(char *curpath)
{
	char		*match;
	DIR			*dir;

	dir = NULL;
	while ((match = ft_strstr(curpath, "/..")))
	{
		if (match == curpath)
			ft_strcpy(curpath, curpath + 3);
		else
		{
			*(++match) = '\0';
			if ((dir = opendir(curpath)) == NULL)
				return (-1);
			closedir(dir);
			*(match - 1) = '\0';
			ft_strcpy(ft_strrchr(curpath, '/'), match + 2);
		}
	}
	if ((curpath)[0] == '\0')
	{
		(curpath)[0] = '/';
		(curpath)[1] = '\0';
	}
	return (0);
}
