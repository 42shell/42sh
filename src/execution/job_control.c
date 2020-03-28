/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/14 17:52:18 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

//job.c

t_process	*process_new(t_node *ast, int stdin, int stdout)
{
	t_process	*process;

	process = (t_process *)ft_xmalloc(sizeof(t_process));
	process->stdin = stdin;
	process->stdout = stdout;
	process->ast = ast;
	return (process);
}

t_job	*job_new(t_node *ast, int stdin, int stdout)
{
	t_job	*job;

	job = (t_job *)ft_xmalloc(sizeof(t_job));
	job->stdin = stdin;
	job->stdout = stdout;
	job->ast = ast;
	return (job);
}

void	process_del(t_process **process)
{
	t_process	*next;

	while (*process)
	{
		next = (*process)->next;
		free(*process);
		*process = next;
	}
}

void	job_del(t_job **job)
{
	t_job	*next;

	while (*job)
	{
		process_del(&(*job)->processes);
		ast_del(&(*job)->ast);
		next = (*job)->next;
		free(*job);
		*job = next;
	}
}

/* ************************************************************************** */

//job_utils.c

void		add_job(t_job *job)
{
	job->next = g_shell.jobs;
	g_shell.jobs = job;
}

void		add_process(t_process *process)
{
	if (!g_shell.jobs)
		return ;
	process->next = g_shell.jobs->processes;
	g_shell.jobs->processes = process;
}

void		remove_job_from_list(pid_t pgid)
{
	t_job	*job;
	t_job	*prev;

	prev = NULL;
	if (!(job = g_shell.jobs))
		return ;
	while (job)
	{
		if (job->pgid == pgid)
		{
			if (prev)
				prev->next = job->next;
			else
				g_shell.jobs = job->next;
		}
		prev = job;
		job = job->next;
	}
}

t_job	*get_job(pid_t pgid)
{
	(void)pgid;
	return (NULL);
}

/* ************************************************************************** */

//job_status.c

void	mark_job_as_running(t_job *job)
{
	t_process *process;

 	process = job->processes;
	while (process)
	{
		process->stopped = false;
		process = process->next;
	}
	job->notified = false;
}

bool	job_is_stopped(t_job *job)
{
	t_process	*process;

	process = job->processes;
	while (process)
	{
		if (!process->stopped)
			return (false);
		process = process->next;
	}
	return (true);
}

bool	job_is_done(t_job *job)
{
	t_process	*process;

	process = job->processes;
	while (process)
	{
		if (!process->done)
			return (false);
		process = process->next;
	}
	return (true);
}

void	set_process_status(t_process *process, int status)
{
	process->status = status;
	if (WIFSTOPPED(status))
		process->stopped = 1;
	else
	{
		process->done = true;
		if (WIFSIGNALED(status))
			ft_dprintf(2, "%d: Terminated by signal %d.\n",
			(int)process->pid, WTERMSIG(process->status));
	}
}

int		mark_status(pid_t pid, int status)
{
	t_job		*job;
	t_process	*process;

	job = g_shell.jobs;
	while (job)
	{
		process = job->processes;
		while (process)
		{
			if (process->pid == pid)
			{
				set_process_status(process, status);
				return (0);
			}
			process = process->next;
		}
		job = job->next;
	}
	return (-1);
}

void	update_status(void)
{
	pid_t	pid;
	int		status;

	pid = 0;
	while ((pid = waitpid(WAIT_ANY, &status, WNOHANG | WUNTRACED)) > 0)
	{
		if (mark_status(pid, status) < 0)
		{
			ft_dprintf(2, "42sh: process %d not found.\n", pid);
			break ;
		}
	}
	if (pid < 0 && errno != ECHILD)
		ft_dprintf(2, "42sh: waitpid: unexpected error.\n", pid);
}

/* ************************************************************************** */

//job_control.c

void	continue_job(t_job *job, bool bg)
{
	mark_job_as_running(job);
	if (bg)
		put_job_bg(job, true);
	else
    	put_job_fg(job, true);
}

void	notif_jobs(void)
{
	t_job	*job;
	t_job	*next;

	update_status();
	job = g_shell.jobs;
	while (job)
	{
		next = job->next;
		if (job_is_done(job))
		{
			if (job->bg)
				printf("%d done.\n", job->pgid);
			remove_job_from_list(job->pgid);
			process_del(&job->processes);
			ast_del(&job->ast);
			free(job);
		}
		else if (job_is_stopped(job) && !job->notified)
		{
			printf("%d stopped.\n", job->pgid);
			job->notified = true;
		}
		job = next;
	}
}

void	wait_for_job(t_job *job)
{
	pid_t		pid;
	int			status;

	pid = 0;
	status = 0;
	if (!job->processes)
		return ;
	while (!job_is_done(job))
	{
		pid = waitpid(WAIT_ANY, &status, WUNTRACED);
		if (pid > 0 && mark_status(pid, status) < 0)
		{
			ft_dprintf(2, "42sh: process %d not found.\n", pid);
			break ;
		}
		else if (pid < 0)
		{
			ft_dprintf(2, "42sh: waitpid: unexpected error.\n", pid);
			break ;
		}
	}
	g_last_exit_st = WEXITSTATUS(job->processes->status);
}

void	put_job_fg(t_job *job, bool cont)
{
	job->bg = false;
	tcsetpgrp(STDIN_FILENO, job->pgid);
	if (cont)
	{
		kill(-job->pgid, SIGCONT);
		tcsetattr (STDIN_FILENO, TCSADRAIN, &job->tmodes);
	}
	wait_for_job(job);
	tcsetpgrp(STDIN_FILENO, g_shell.pgid);
	tcgetattr(STDIN_FILENO, &job->tmodes);
	tcsetattr(STDIN_FILENO, TCSADRAIN, &g_shell.tmodes);
}

void	put_job_bg(t_job *job, bool cont)
{
	job->bg = true;
	if (cont)
		kill(-job->pgid, SIGCONT);
}