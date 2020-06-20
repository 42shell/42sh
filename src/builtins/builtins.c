/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 13:07:29 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/29 02:25:59 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			builtin_echo(char **argv, __attribute__((unused)) t_array *env)
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

/*
** "42sh: error" its not a binary
*/

int			builtin_setenv(char **argv, t_array *env)
{
	if (argv[1] == NULL)
		return (builtin_env(argv, env));
	else if (argv[2] && argv[3])
		write(2, "setenv: Too many arguments.\n", 28);
	else if (!is_valid_var_name(argv[1]))
		write(2, "setenv: Invalid variable name\n", 30);
	else
	{
		set_var(argv[1], argv[2], V_EXPORT);
		return (0);
	}
	return (1);
}

int			builtin_unsetenv(char **argv,
			__attribute__((unused)) t_array *env)
{
	int		i;
	t_var	*var;

	i = 1;
	while (argv[i])
	{
		if ((var = ht_get(g_shell.vars, argv[i]))
		&& var->attributes & V_EXPORT)
			unset_var(argv[i]);
		i++;
	}
	return (0);
}

static bool	there_are_stopped_jobs(void)
{
	t_job		*job;
	static bool	notif = false;

	update_status();
	if (!g_shell.jobs)
		return (false);
	job = g_shell.jobs->next;
	while (job)
	{
		if (job_is_stopped(job) && !notif)
		{
			notif = true;
			ft_dprintf(2, "42sh: exit: There are stopped jobs.\n");
			return (true);
		}
		job = job->next;
	}
	return (false);
}

void		builtin_exit(char **argv,
			__attribute__((unused)) t_array *env)
{
	int status;

	if (there_are_stopped_jobs())
		return ;
	if (!argv || argv[1] == NULL)
		status = g_last_exit_st;
	else if (argv[2] != NULL)
	{
		write(2, "42sh: exit: too many arguments\n", 31);
		return ;
	}
	else if (!ft_strisnbr(argv[1]))
	{
		write(2, "42sh: exit: numeric argument required\n", 38);
		return ;
	}
	else
		status = ft_atoi(argv[1]);
	if (g_shell.interactive_mode)
		tcsetattr(STDIN_FILENO, TCSADRAIN, &g_shell.tmodes);
	exit(status);
}
