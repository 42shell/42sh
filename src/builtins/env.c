/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 17:03:57 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/17 05:04:09 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "shell.h"
#include "shell.h"

#define EMPTY_ENV 	1
#define NEW_PATH	2

int		get_env_options(int argc, char **argv, int *options)
{
	int		c;
	int		ret;

	ret = 1;
	while ((c = get_opt(argc, (unsigned char **)argv)) != -1)
	{
		*options = (c == '\0' || c == 'i') ? EMPTY_ENV : 0;
		if (c != '\0' && c != 'i' && ret == 1)
		{
			ft_dprintf(2, "env: illegal option -- %c\n", c);
			ret = 0;
		}
	}
	return (ret);
}

int		modify_env(char **argv, t_array *env, int *options)
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

int		print_env(t_array *env)
{
	size_t	i;

	i = 0;
	while (i < env->size)
	{
		ft_putstr_fd(env->array[i++], 1);
		write(1, "\n", 1);
	}
	array_destroy(env);
	return (0);
}

int		finish_env(char **argv, int i, t_array *new_env)
{
	int		ret;

	ret = 0;
	if (argv[i] == NULL)
	{
		ret = 127;
		argv[i] = ft_strdup("");
	}
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
	array_destroy(new_env);
	return (ret);
}

/*
** exec_binary(get_exec_path(argv[i])), argv[i], NULL, new_env->env);
*/

int		builtin_env(char **argv, t_array *env)
{
	int				options;
	int				argc;
	t_array			*new_env;
	int				i;
	char			*cmd_path;

	options = 0;
	argc = 0;
	while (argv[argc])
		argc++;
	if (!get_env_options(argc, argv, &options))
		return (1);
	new_env = (options & EMPTY_ENV) ? env_dup(argv + argc)
									: env_dup((char **)env->array);
	i = modify_env(argv, new_env, &options);
	if (argv[i] == NULL)
		return (print_env(new_env));
	cmd_path = (options & NEW_PATH) ? get_exec_path(argv[i], new_env)
		: get_exec_path(argv[i], env);
	if (cmd_path == NULL)
		ft_dprintf(2, "env: '%s': No such file or directory\n", argv[i]);
	free(argv[i]);
	argv[i] = cmd_path;
	return (finish_env(argv, i, new_env));
}
