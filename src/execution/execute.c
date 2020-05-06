/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/06 15:26:27 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			exec_builtin(char **argv, t_env *temp_env)
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

int			exec_binary(char **argv, t_env *temp_env)
{
	char	*path;

	if (!(path = get_exec_path(argv[0], temp_env)))
	{
		ft_dprintf(2, "42sh: %s: command not found\n", argv[0]);
		exit (1); //code?
	}
	else if (execve(path, argv, temp_env->env) == -1)
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
	t_env *temp_env;

	if (set_redir(simple, true) != 0)
		return (1);
	if (simple->argv)
	{
		temp_env = export_env(g_shell.vars);
		if (is_builtin(simple->argv[0]))
			exec_builtin(simple->argv, temp_env);
		else
			exec_binary(simple->argv, temp_env);
		free_arr(temp_env->env);
		free(temp_env);
	}
	restore_fds();
	return (0);
}
