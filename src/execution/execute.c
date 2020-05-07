/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/06 22:23:32 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			exec_builtin(char **argv, t_array *temp_env)
{
	int		ret;

	ret = 0;
	if (ft_strequ(argv[0], "env"))
		ret = builtin_env(argv, temp_env);
	else if (ft_strequ(argv[0], "exit"))
		builtin_exit(argv);
	else if (ft_strequ(argv[0], "unsetenv"))
		ret = builtin_unsetenv(argv);
	else if (ft_strequ(argv[0], "setenv"))
		ret = builtin_setenv(argv, temp_env);
	else if (ft_strequ(argv[0], "echo"))
		ret = builtin_echo(argv);
	else if (ft_strequ(argv[0], "cd"))
		ret = builtin_cd(argv);
	else if (ft_strequ(argv[0], "export"))
		builtin_export(argv);
	else if (ft_strequ(argv[0], "unset"))
		builtin_unset(argv);
	else if (ft_strequ(argv[0], "set"))
		builtin_set();
	else if (ft_strequ(argv[0], "fg"))
		ret = builtin_fg(argv);
	else if (ft_strequ(argv[0], "bg"))
		ret = builtin_bg(argv);
	else if (ft_strequ(argv[0], "jobs"))
		ret = builtin_jobs(argv);
	g_last_exit_st = ret;
	return (0); //errors ?
}

int			exec_binary(char **argv, t_array *temp_env)
{
	char		*path;
	struct stat	b;

	if (!(path = get_exec_path(argv[0], temp_env)))
	{
		ft_dprintf(2, "42sh: %s: command not found\n", argv[0]);
		exit(127);
	}
	execve(path, argv, (char **)temp_env->array);
	if (stat(path, &b) != 0)
	{
		ft_dprintf(2, "42sh: %s: command not found\n", argv[0]);
		exit(127);
	}
	if (!(S_IXUSR & b.st_mode))
		ft_dprintf(2, "42sh: %s: Permission denied\n", argv[0]);
	else if (S_ISDIR(b.st_mode))
		ft_dprintf(2, "42sh: %s: Is a directory\n", argv[0]);
	else
		ft_dprintf(2, "42sh: %s: cannot execute command\n", argv[0]);
	exit(126);
}

int			exec_simple_command(t_simple_cmd *simple)
{
	t_array *temp_env;

	if (set_redir(simple, true) != 0)
	{
		restore_fds();
		return (g_last_exit_st = 1);
	}
	if (simple->argv)
	{
		temp_env = export_env(g_shell.vars);
		set_temp_env_variables(simple, temp_env);
		if (is_builtin(simple->argv[0]))
			exec_builtin(simple->argv, temp_env);
		else
			exec_binary(simple->argv, temp_env);
		array_destroy(temp_env);
	}
	else
	{
		set_local_variables(simple);
		g_last_exit_st = 0;
	}
	restore_fds();
	return (0);
}
