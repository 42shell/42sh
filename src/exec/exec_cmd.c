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
int			exec_builtin(t_node *command_node, char **argv) //bool free_argv
{
	if (set_redir(cmd, true) > 0)
	{
		free_argv(argv);
		restore_fds();
		return (1);
	}
	if (ft_strequ(argv->argv[0], "env"))
		builtin_env(argv->argv, env);
	else if (ft_strequ(argv->argv[0], "exit"))
		builtin_exit(argv->argv);
	else if (ft_strequ(argv->argv[0], "unsetenv"))
		builtin_unsetenv(argv->argv, env);
	else if (ft_strequ(argv->argv[0], "setenv"))
		builtin_setenv(argv->argv, env);
	else if (ft_strequ(argv->argv[0], "echo"))
		builtin_echo(argv->argv);
	else if (ft_strequ(argv->argv[0], "cd"))
		builtin_cd(argv->argv, env);
	restore_fds();
	if (free_av)
		free_argv(argv);
	return (0);
}

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

int			exec_command(t_command *command)
{
	pid_t		pid;
	int			status;
 
	//if (is_builtin(argv[0])) //is_builtin(t_command *)
	//	return (exec_builtin(command, argv));
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