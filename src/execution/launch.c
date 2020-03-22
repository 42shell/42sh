/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/14 17:52:18 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
static int	launch_builtin(t_process *process, bool bg)
{
	if (!bg && !process->next)
	{
		set_redir(process, true); //always backup and restore
		return (exec_builtin(process->argv, g_env->env));
	}
	else if ((process->pid = fork()) == -1)
	{
		ft_dprintf(2, "42sh: too many processes\n");
		kill(STDIN_FILENO, SIGABRT);
	}
	else if (process->pid == 0)
	{
		process->pid = getpid();
		printf("child %d: %d\n", i, process->pid);
		set_redir(process, true); //always backup and restore
		exit(exec_builtin(process->argv, g_env->env));
	}
	printf("parent %d: %d\n", i, process->pid);
	return (0);
}
*/

static void	set_child_pgid(t_process *process, pid_t *pgid, bool bg)
{
	process->pid = getpid();
	*pgid = *pgid ? *pgid : process->pid;
	setpgid(process->pid, *pgid);
	if (!bg)
		tcsetpgrp(STDIN_FILENO, *pgid);
}

int			launch_process(t_process *process, pid_t *pgid, bool bg)
{
	//expand(process);
	process->argv = get_argv(process);
	if (!process->next && !bg
	&& is_builtin(process->argv[0]))
	{
		set_redir(process, true); //always backup and restore
		return (exec_builtin(process->argv, g_env->env));
	}
	else if ((process->pid = fork()) == -1)
	{
		ft_dprintf(2, "42sh: too many processes\n");
		kill(STDIN_FILENO, SIGABRT);
	}
	else if (process->pid == 0)
	{
		set_child_pgid(process, pgid, bg);
		set_redir(process, true); //always backup and restore
		if (is_builtin(process->argv[0]))
			exit(exec_builtin(process->argv, g_env->env));
		exit(exec_binary(process->argv, g_env->env));//?
	}
	*pgid = *pgid ? *pgid : process->pid;
	setpgid(process->pid, *pgid);
	return (0);
}

/*
** loop through the processes:
** -we fork the process
** -we call launch_process()
** we wait for pipeline
*/

int			launch_pipeline(t_pipeline *pipeline, pid_t *pgid, bool bg)
{
	t_process	*process;

	process = pipeline->processes;
	while (process)
	{
		if (process->next)
		{
			if (set_pipe(process) == -1)
			{
				ft_dprintf(2, "42sh: failed to create pipe\n");
				return (-1); //pipe_error()
			}
			launch_process(process, pgid, bg);
			close_pipe(process);
		}
		else
			launch_process(process, pgid, bg);
		process = process->next;
	}
	return (0);
}

/*
** loop through the jobs:
** -we fork the job
** -we call launch_job in the child
** -we wait for the job or not
*/

int				launch_list(t_list *list)
{
	t_job	*job;

	job = list->jobs;
	while (job)
	{
		add_job(job);
		job->curr_pipeline = job->pipelines;
		launch_pipeline(job->curr_pipeline, &job->pgid, job->bg);
		if (job->bg)
			return (0);//put_job_bg(job);
		else
			put_job_fg(job, false);
		if (job)
			job = job->next;
	}
	return (0);
}