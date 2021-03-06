/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 14:02:44 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/17 04:54:19 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

#define CD_P		1
#define PRINT_DIR	2

char	*get_pwd(void)
{
	static char	buf[PATH_MAX + 1];
	char		*pwd;
	char		*cwd;
	struct stat	pwd_stat;
	struct stat cwd_stat;

	pwd = ft_strdup(get_var_value("PWD"));
	if (pwd && pwd[0])
	{
		remove_dots(pwd);
		remove_dotdots(pwd);
		if (ft_strequ(pwd, "..") || ft_strequ(pwd, "."))
			pwd[0] = '\0';
	}
	cwd = getcwd(buf, PATH_MAX + 1);
	if (cwd == NULL || stat(cwd, &cwd_stat) < 0)
		ft_putstr_fd("cd: PWD not set\n"
						"getcwd: could not get current dir\n"
						, 2);
	if (!(pwd == NULL || pwd[0] == '\0' || stat(pwd, &pwd_stat) < 0
			|| cwd_stat.st_ino != pwd_stat.st_ino
			|| cwd_stat.st_dev != pwd_stat.st_dev))
		ft_strcpy(cwd, pwd);
	free(pwd);
	return (cwd);
}

char	*get_home_dir(t_array *env)
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

void	append_curpath_to_pwd(char **curpath)
{
	char *new;
	char *pwd;

	if ((pwd = get_pwd()))
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

	while ((match = find_dot(curpath)))
		ft_strcpy(match, match + 2);
}

int		remove_dotdots(char *curpath)
{
	char		*match;
	DIR			*dir;

	dir = NULL;
	while ((match = find_dotdot(curpath)))
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
			if (ft_strrchr(curpath, '/'))
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
