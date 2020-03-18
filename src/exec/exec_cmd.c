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

this goes in builtins/env

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

/*
int			exec_builtin(t_command *cmd, char **env)
{
	(void)env;
	if (set_redir(cmd, true) == 0)
	{
		//if (ft_strequ(argv[0], "env"))
		//	builtin_env(argv, env);
		if (ft_strequ(cmd->argv[0], "exit"))
			builtin_exit(cmd->argv);
		else if (ft_strequ(cmd->argv[0], "unsetenv"))
			builtin_unsetenv(cmd->argv);
		else if (ft_strequ(cmd->argv[0], "setenv"))
			builtin_setenv(cmd->argv);
		else if (ft_strequ(cmd->argv[0], "echo"))
			builtin_echo(cmd->argv);
		else if (ft_strequ(cmd->argv[0], "cd"))
			builtin_cd(cmd->argv);
	}
	restore_fds();
	return (0);
}

int			exec_binary(t_command *cmd, char **env)
{
	pid_t		pid;
	int			status;
 
	if ((pid = fork()) == -1)
		return (-1); // code ?
	signal(SIGINT, interrupt_fork);
	if (pid == 0)
	{
		if (set_redir(cmd, false) != 0)
			exit(1);//code ?
		if (execve(cmd->path, cmd->argv, env) == -1)
		{
			ft_dprintf(2, "42sh: %s: cannot execute command\n", cmd->argv[0]);
			exit(g_last_exit_st);
		}
		exit(0);
	}
	wait(&status);
	g_last_exit_st = WIFEXITED(status) ? WEXITSTATUS(status) : g_last_exit_st;
	return (g_last_exit_st);
}
*/