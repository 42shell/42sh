/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:52:31 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/07 00:36:28 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
int			exec_command_env(char **argv, t_env *env)
{
	pid_t		pid;
	int			status;

	pid = fork();
	signal(SIGINT, interrupt_fork);
	if (pid == 0)
	{
		if ((execve(argv[0], argv, env->env) == -1))
		{
			ft_dprintf(2, "env: %s: No such file or directory\n", argv[0]);
			exit(g_last_exit_st);
		}
		exit(0);
	}
	wait(&status);
	g_last_exit_st = WIFEXITED(status) ? WEXITSTATUS(status)
		: g_last_exit_st;
	return (g_last_exit_st);
}
*/

int			exec_builtin(t_command *command)
{
	if (set_redir(command, true) == 0)
	{
		//if (ft_strequ(command->argv[0], "env"))
		//	builtin_env(command->argv, g_env);
		if (ft_strequ(command->argv[0], "exit"))
			builtin_exit(command->argv);
		else if (ft_strequ(command->argv[0], "unsetenv"))
			builtin_unsetenv(command->argv);
		else if (ft_strequ(command->argv[0], "setenv"))
			builtin_setenv(command->argv);
		else if (ft_strequ(command->argv[0], "echo"))
			builtin_echo(command->argv);
		else if (ft_strequ(command->argv[0], "cd"))
			builtin_cd(command->argv);
	}
	restore_fds();
	return (0);
}

int			exec_binary(t_command *command)
{
	pid_t		pid;
	int			status;
 
	if (!(command->path = get_exec_path(command->argv[0])))
		return (command_not_found(command->argv[0]));
	pid = fork();
	signal(SIGINT, interrupt_fork);
	if (pid == 0)
	{
		if (set_redir(command, false) > 0)
			exit(1);
		if (execve(command->path, command->argv, g_env->env) == -1)
		{
			ft_dprintf(2, "42sh: %s: cannot execute command\n", command->argv[0]);
			exit(g_last_exit_st);
		}
		exit(0);
	}
	wait(&status);
	g_last_exit_st = WIFEXITED(status) ? WEXITSTATUS(status) : g_last_exit_st;
	return (g_last_exit_st);
}