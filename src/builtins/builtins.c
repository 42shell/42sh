/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 13:07:29 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/24 15:29:16 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

extern int g_last_exit_st;

int		builtin_echo(char **argv)
{
	int i;

	i = 1;
	while (argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1] != NULL)
			write(1, " ", 1);
		i++;
	}
	write(1, "\n", 1);
	return (0);
}

int		builtin_setenv(char **argv, t_env *env)
{
	if (argv[1] == NULL)
		return (builtin_env(argv, env));
	if (argv[2] && argv[3])
		write(2, "setenv: Too many arguments.\n", 28);
	else if (!is_valid_var_name(argv[1]))
		write(2, "setenv: Invalid variable name\n", 30);
	else
	{
		set_env_var(argv[1], argv[2], env);
		return (0);
	}
	return (1);
}

int		builtin_unsetenv(char **argv, t_env *env)
{
	int i;

	i = 1;
	while (argv[i])
		remove_env_var(argv[i++], env);
	return (0);
}

void	builtin_exit(char **argv)
{
	int i;

	if (!argv)
		exit(g_last_exit_st);
	if (argv[1] == NULL)
		exit(g_last_exit_st);
	i = 0;
	while (argv[1][i])
	{
		if (!ft_isdigit(argv[1][i++]))
		{
			write(2, "42sh: exit: numeric argument required\n", 38);
			return ;
		}
	}
	if (argv[2] != NULL)
	{
		write(2, "42sh: exit: too many arguments\n", 31);
		return ;
	}
	i = ft_atoi(argv[1]);
	exit(i);
}
