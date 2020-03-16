/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 17:03:57 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/17 14:55:26 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "shell.h"
#include "shell.h"

#define EMPTY_ENV 	1
#define NEW_PATH	2

extern int	g_last_exit_st;

int		get_env_options(int argc, char **argv, int *options)
{
	int		c;
	int		ret;

	ret = 1;
	while ((c = get_opt(argc, argv)) != -1)
	{
		*options = (c == '\0' || c == 'i') ? EMPTY_ENV : 0;
		if (c != '\0' && c != 'i' && ret == 1)
		{
			ft_putstr_fd("env: illegal option -- ", 2);
			ft_putc(c);
			ft_putc('\n');
			ret = 0;
		}
	}
	return (ret);
}

int		modify_env(char **argv, t_env *env, int *options)
{
	int		i;
	char	*value;

	i = 1;
	while (argv[i] && argv[i][0] == '-')
		if (ft_strcmp(argv[i++], "--") == 0)
			break ;
	while (argv[i] && (value = ft_strchr(argv[i], '=')))
	{
		*value = '\0';
		if (ft_strcmp(argv[i], "PATH") == 0)
			*options |= NEW_PATH;
		set_env_var(argv[i], value + 1, env);
		i++;
	}
	return (i);
}

int		print_env(t_env *env)
{
	int i;

	i = 0;
	while (env->env[i])
	{
		ft_putstr_fd(env->env[i++], 1);
		write(1, "\n", 1);
	}
	free_arr(env->env);
	return (0);
}

int		finish_env(char **argv, int i, t_env *new_env)
{
	int		ret;

	ret = 0;
	if (argv[i] == NULL)
		ret = 127;
	else if (access(argv[i], F_OK) == -1)
	{
		ret = 127;
		ft_putstr_fd("env: ", 2);
		ft_putstr_fd(argv[i], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
	}
	else
	{
		exec_command_env(argv + i, new_env);
		ret = g_last_exit_st;
	}
	free_arr(new_env->env);
	return (ret);
}

/*
** just
** -use a t_env *g_env_tmp to save the real env,
** -set g_env to new_env
** like this we don t need to pass an env parameter
*/

int		builtin_env(char **argv, t_env *env)
{
	int				options;
	int				argc;
	t_env			new_env;
	int				i;
	char			*cmd_path;

	options = 0;
	argc = 0;
	while (argv[argc])
		argc++;
	if (!get_env_options(argc, argv, &options))
		return (1);
	new_env = (options & EMPTY_ENV) ? env_dup(argv + argc) : env_dup(env->env);
	i = modify_env(argv, &new_env, &options);
	if (argv[i] == NULL)
		return (print_env(&new_env));
	cmd_path = (options & NEW_PATH) ? get_executable_path(argv[i], &new_env)
		: get_executable_path(argv[i], env);
	if (cmd_path == NULL)
		ft_dprintf(2, "env: '%s': No such file or directory\n", argv[i]);
	free(argv[i]);
	argv[i] = cmd_path;
	return (finish_env(argv, i, &new_env));
}
