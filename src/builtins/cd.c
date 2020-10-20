/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 13:50:00 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/17 05:04:49 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

#define CD_P		1
#define PRINT_DIR	2
#define NO_OLDPWD	1

char	*get_cd_dir(char **argv, int *options)
{
	int		i;
	char	*ret;

	i = 1;
	ret = NULL;
	while (argv[i] && argv[i][0] == '-' && argv[i][1] != '\0')
		if (ft_strcmp(argv[i++], "--") == 0)
			break ;
	if (argv[i] != NULL)
	{
		if (ft_strcmp(argv[i], "-") == 0)
		{
			if ((ret = get_var_value("OLDPWD")) == NULL)
			{
				ft_putstr_fd("cd: OLDPWD not set\n", 2);
				return ((char *)NO_OLDPWD);
			}
			*options = *options | PRINT_DIR;
		}
		else
			ret = argv[i];
	}
	return (ft_strdup(ret));
}

int		get_cd_options(char **argv, int *options)
{
	int		argc;
	int		ret;
	int		c;

	argc = 0;
	ret = 0;
	while (argv[argc])
		argc++;
	while ((c = get_opt(argc, (unsigned char **)argv)) != -1)
	{
		*options = (c == 'P') ? CD_P : *options;
		*options = (c == 'L') ? 0 : *options;
		if (c != 'L' && c != 'P' && ret == 0)
		{
			ft_putstr_fd("cd: illegal option -- ", 2);
			write(2, &c, 1);
			write(2, "\n", 1);
			ret = -1;
		}
	}
	return (ret);
}

/*
** This is step 5 in the posix cd algorithm.
*/

bool	get_curpath_in_cdpath(char *dir, char **curpath, int *options)
{
	char		*tmp;
	char		**cdpath;
	struct stat	buf;
	int			i;

	if (ft_strequ(dir, ".") || ft_strequ(dir, "..")
	|| (ft_strstr(dir, "./") == dir) || (ft_strstr(dir, "../") == dir)
	|| (tmp = get_var_value("CDPATH")) == NULL || tmp[0] == '\0')
		return (false);
	cdpath = split_path(tmp);
	i = 0;
	while (cdpath[i])
	{
		tmp = append_filename(cdpath[i++], dir);
		if (stat(tmp, &buf) == 0 && S_ISDIR(buf.st_mode))
		{
			*curpath = tmp;
			free_arr(cdpath);
			*options |= PRINT_DIR;
			return (true);
		}
		free(tmp);
	}
	free_arr(cdpath);
	return (false);
}

int		finish_cd(char *curpath, char *dir, int options)
{
	int		ret;
	char	*pwd;

	ret = 0;
	if (!(options & CD_P))
	{
		remove_dots(curpath);
		if (remove_dotdots(curpath) == -1)
			ret = 1;
	}
	ret = (chdir(curpath) == -1) ? 1 : ret;
	if (ret == 0)
	{
		set_var("OLDPWD", get_var_value("PWD"), V_EXPORT);
		pwd = getcwd(NULL, 0);
		set_var("PWD", (options & CD_P) ? pwd : curpath, V_EXPORT);
		free(pwd);
	}
	if (options & PRINT_DIR && ret == 0)
		ft_putendl(curpath);
	free(curpath);
	free(dir);
	if (ret == 1)
		ft_putstr_fd("cd: error\n", 2);
	return (ret);
}

int		builtin_cd(char **argv, t_array *env)
{
	char	*dir;
	char	*curpath;
	int		options;

	options = 0;
	if (get_cd_options(argv, &options) == -1)
		return (1);
	if ((dir = get_cd_dir(argv, &options)) == (char *)NO_OLDPWD)
		return (1);
	if (dir == NULL)
	{
		dir = get_home_dir(env);
		if (dir == NULL)
			return (1);
	}
	if (dir[0] == '/' || !get_curpath_in_cdpath(dir, &curpath, &options))
		curpath = ft_strdup(dir);
	if (curpath[0] != '/' && !(options & CD_P))
		append_curpath_to_pwd(&curpath);
	if (curpath == NULL)
	{
		free(dir);
		return (1);
	}
	return (finish_cd(curpath, dir, options));
}
