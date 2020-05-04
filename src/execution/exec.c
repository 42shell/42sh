/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/24 01:04:59 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			exec_builtin(char **argv)
{
	int		ret;

	ret = 0;
	if (ft_strequ(argv[0], "env"))
		ret = builtin_env(argv, g_env);
	else if (ft_strequ(argv[0], "exit"))
		builtin_exit(argv);
	else if (ft_strequ(argv[0], "unsetenv"))
		ret = builtin_unsetenv(argv);
	else if (ft_strequ(argv[0], "setenv"))
		ret = builtin_setenv(argv);
	else if (ft_strequ(argv[0], "echo"))
		ret = builtin_echo(argv);
	else if (ft_strequ(argv[0], "cd"))
		ret = builtin_cd(argv);
	else if (ft_strequ(argv[0], "fg"))
		ret = builtin_fg(argv);
	else if (ft_strequ(argv[0], "bg"))
		ret = builtin_bg(argv);
	else if (ft_strequ(argv[0], "jobs"))
		ret = builtin_jobs(argv);
	g_last_exit_st = ret;
	return (0); //errors ?
}

int			exec_binary(char **argv, char **env)
{
	char	*path;

	if (!(path = get_exec_path(argv[0], g_env)))
	{
		ft_dprintf(2, "42sh: %s: command not found\n", argv[0]);
		exit (1); //code?
	}
	else if (execve(path, argv, env) == -1)
	{
		ft_dprintf(2, "42sh: %s: cannot execute command\n", argv[0]);
		free(path);
		return (1); //code ?
	}
	free(path);
	return (0);
}

int			exec_simple_command(t_simple_cmd *simple)
{
	if (set_redir(simple, true) != 0)
		return (1);
	if (simple->argv)
	{
		if (is_builtin(simple->argv[0]))
		{
			exec_builtin(simple->argv);
			restore_fds();
		}
		else
			exec_binary(simple->argv, g_env->env);
	}
	else
		restore_fds();
	return (0);
}