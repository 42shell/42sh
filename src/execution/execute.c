/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/25 02:47:41 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

bool g_already_forked;

int			exec_builtin(char **argv, t_array *temp_env)
{
	int				ret;
	t_builtin_func	builtin;

	ret = 0;
	builtin = ht_get(g_builtins, argv[0]);
	if (builtin == NULL)
		return (1);
	ret = builtin(argv, temp_env);
	if (g_already_forked)
		exit(ret);
	g_last_exit_st = ret;
	return (0);
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
	if (S_ISDIR(b.st_mode))
		ft_dprintf(2, "42sh: %s: Is a directory\n", argv[0]);
	else if (!(S_IXUSR & b.st_mode))
		ft_dprintf(2, "42sh: %s: Permission denied\n", argv[0]);
	else
		ft_dprintf(2, "42sh: %s: cannot execute command\n", argv[0]);
	exit(126);
}

int			exec_simple_command(t_simple_cmd *simple)
{
	t_array		*temp_env;

	if (set_redir(simple->redirs, true) != 0)
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

int			exec_group_command(t_group_cmd *group)
{
	t_job		*job;

	if (group->list && group->list->next)
		g_already_forked = false;
	while (group->list)
	{
		job = job_new(group->list, STDIN_FILENO, STDOUT_FILENO);
		job->pgid = g_shell.jobs->pgid;
		if (group->list->sep == AMPERSAND)
			job->bg = true;
		launch_job(job);
		group->list = group->list->next;
	}
	return (0);
}
