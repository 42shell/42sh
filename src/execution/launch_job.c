/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_job.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/10 16:24:11 by fratajcz         ###   ########.fr       */
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

static pid_t	fork_child(int in, int out, int to_close)
{
	pid_t	pid;

	if ((pid = fork()) == -1)
		return (-1);
	else if (pid == 0)
	{
		if (to_close)
			close(to_close);
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

static int		exec_simple_cmd(t_simple_cmd *simple)
{
	char			**argv;

	set_redir(simple, true);
	argv = simple->argv == NULL ? get_argv(simple) : simple->argv;
	if (is_builtin(argv[0]))
		return (exec_builtin(argv, g_env->env));
	return (exec_binary(argv, g_env->env));
}

int				launch_process(t_process *process, int to_close, bool subshell)
{
	pid_t	pid;

	if ((pid = fork_child(process->stdin, process->stdout, to_close)) == -1)
		return (-1);
	else if (pid == 0)
	{
		process->pid = getpid();
		if (!g_shell.jobs->pgid)
			g_shell.jobs->pgid = process->pid;
		setpgid(process->pid, g_shell.jobs->pgid);
		if (!g_shell.jobs->bg)
			tcsetpgrp(STDIN_FILENO, g_shell.jobs->pgid);
		reset_signals();
		if (process->command->type == SIMPLE)
			exec_simple_cmd(process->command->value.simple);
		else
			eval_command(process->command);
		if (subshell)//job_new(), launch_job(); for real subshells
			wait_for_job(g_shell.jobs);
		exit(0);
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

int				launch_job(t_job *job)
{
	t_process	*process;

	if (job->bg)
	{
		process = process_new(job->command, STDIN_FILENO, STDOUT_FILENO);
		launch_process(process, 0, true);
		put_job_bg(job, false);
		ft_printf("[%d] %d\n", job->id + 1, job->pgid);
	}
	else
	{
		eval_command(job->command);
		put_job_fg(job, false);
	}
	return (0);
}
