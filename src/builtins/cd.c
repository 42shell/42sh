/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 13:50:00 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/28 17:48:22 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

#define CD_P		1
#define HAS_ARG		2
#define PRINT_DIR	4
#define NO_OLDPWD	1

char	*get_cd_dir(char **argv, int *options, t_env *env)
{
	int		i;
	char	*ret;

	i = 1;
	ret = NULL;
	while (argv[i] && argv[i][0] == '-' && argv[i][1] != '\0')
		if (ft_strcmp(argv[i++], "--") == 0)
			break ;
	if (argv[i] != NULL)
		*options |= HAS_ARG;
	if (*options & HAS_ARG)
	{
		if (ft_strcmp(argv[i], "-") == 0)
		{
			if ((ret = get_env_var("OLDPWD", env)) == NULL)
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
	char	c;

	argc = 0;
	ret = 0;
	while (argv[argc])
		argc++;
	while ((c = get_opt(argc, argv)) != -1)
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

bool	get_curpath_in_cdpath(char *dir, char **curpath, t_env *env
		, int *options)
{
	char		*tmp;
	char		**cdpath;
	struct stat	buf;
	int			i;

	if (ft_strequ(dir, ".") || ft_strequ(dir, "..")
			|| (ft_strstr(dir, "./") == dir) || (ft_strstr(dir, "../") == dir)
			|| (tmp = get_env_var("CDPATH", env)) == NULL)
		return (false);
	cdpath = split_path(tmp);
	i = 0;
	while (cdpath[i])
	{
		tmp = append_filename(cdpath[i++], dir);
		if (lstat(tmp, &buf) == 0 && S_ISDIR(buf.st_mode))
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

int		finish_cd(char *curpath, char *dir, int options, t_env *env)
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
		set_env_var("OLDPWD", get_env_var("PWD", env), env);
		pwd = getcwd(NULL, 0);
		set_env_var("PWD", (options & CD_P) ? pwd : curpath, env);
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

int		builtin_cd(char **argv, t_env *env)
{
	char	*dir;
	char	*curpath;
	int		options;

	options = 0;
	if (get_cd_options(argv, &options) == -1)
		return (1);
	if ((dir = get_cd_dir(argv, &options, env)) == (char *)NO_OLDPWD)
		return (1);
	if (!(options & HAS_ARG) && !(dir = get_home_dir(env)))
		return (1);
	if (dir[0] == '/' || !get_curpath_in_cdpath(dir, &curpath, env, &options))
		curpath = ft_strdup(dir);
	if (curpath[0] != '/' && !(options & CD_P))
		append_curpath_to_pwd(&curpath, env);
	if (curpath == NULL)
	{
		free(dir);
		return (1);
	}
	return (finish_cd(curpath, dir, options, env));
}
