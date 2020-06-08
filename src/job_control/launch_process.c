/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/09 15:25:18 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void		reset_signals(void)
{
	prctl(PR_SET_PDEATHSIG, SIGTERM);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTTIN, SIG_DFL);
	signal(SIGTTOU, SIG_DFL);
	signal(SIGTSTP, SIG_DFL);
}

static pid_t	fork_child(int in, int out, int fd_to_close)
{
	pid_t	pid;

	if ((pid = fork()) == -1)
	{
		kill(0, SIGABRT);
		ft_dprintf(2, "42sh: Failed to fork, aborting\n");
		return (-1);
	}
	else if (pid == 0)
	{
		if (fd_to_close)
			close(fd_to_close);
		if (in != STDIN_FILENO)
			dup2(in, STDIN_FILENO);
		if (out != STDOUT_FILENO)
			dup2(out, STDOUT_FILENO);
	}
	if (in != STDIN_FILENO)
		close(in);
	if (out != STDOUT_FILENO)
		close(out);
	return (pid);
}

static void		set_child_attr(t_process *process)
{
	process->pid = getpid();
	if (!g_shell.jobs->pgid)
		g_shell.jobs->pgid = process->pid;
	setpgid(process->pid, g_shell.jobs->pgid);
	if (!g_shell.jobs->bg)
		tcsetpgrp(STDIN_FILENO, g_shell.jobs->pgid);
	//case "(ls && subshell) | cat" g_shell.jobs contains cat process
	//and we dont want to wait for it in the subshell
	process_del(&g_shell.jobs->processes);
	g_job_control_enabled = false;
	g_already_forked = true;
}

int				launch_process(t_process *process, int fd_to_close)
{
	pid_t	pid;

	if ((pid = fork_child(process->stdin, process->stdout, fd_to_close)) == -1)
		return (-1);
	if (pid == 0)
	{
		set_child_attr(process);
		reset_signals();
		eval_command(process->command);
		wait_for_job(g_shell.jobs);
		builtin_exit(NULL, NULL);
	}
	else
	{
		process->pid = pid;
		if (!g_shell.jobs->pgid)
			g_shell.jobs->pgid = pid;
		setpgid(process->pid, g_shell.jobs->pgid);
		add_process(process);
	}
	return (pid);
}
