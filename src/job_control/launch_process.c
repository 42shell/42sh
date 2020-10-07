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

extern int	*g_backups;

static void		reset_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTTIN, SIG_DFL);
	signal(SIGTTOU, SIG_DFL);
	signal(SIGTSTP, SIG_DFL);
	signal(SIGHUP, SIG_DFL);
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

/*
** in case of async group command, jobs corresponding to the compound list
** are added to the current jobs list.
** We can't set job->bg = true, otherwise the subshell will not wait for it to
** complete. g_bg allows us to keep trace of the background state.
** ex: (ls | cat) &
** g_bg is set to true when we fork the "(ls | cat) &" job
** and allows us to know that we are in the background in further forks,
** even if a foreground "ls | cat" job has been added to g_current_jobs.
*/

static void		set_child_attr(t_process *process)
{
	process->pid = getpid();
	if (g_job_control_enabled)
	{
		if (!g_current_jobs->pgid)
			g_current_jobs->pgid = process->pid;
		setpgid(process->pid, g_current_jobs->pgid);
	}
	else
		g_current_jobs->pgid = g_shell.pgid;
	if (!g_bg && g_job_control_enabled)
		tcsetpgrp(STDIN_FILENO, g_current_jobs->pgid);
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
		builtin_exit(NULL, NULL);
	}
	else
	{
		process->pid = pid;
		if (g_job_control_enabled)
		{
			if (!g_current_jobs->pgid)
				g_current_jobs->pgid = pid;
			setpgid(process->pid, g_current_jobs->pgid);
		}
		else
			g_current_jobs->pgid = g_shell.pgid;
		add_process_to_job(g_current_jobs, process);
	}
	return (pid);
}
