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

void	process_delone(t_process **process)
{
	free(*process);
	*process = NULL;
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
		next = (*job)->next;
		free(*job);
		*job = next;
	}
}

void	remove_job_from_list(pid_t pgid)
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

/*
bool	job_is_stopped(t_job *job)
{
	(void)job;
	return (false);
}
*/

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

/* ************************************************************************** */

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

void	notif_jobs(void)
{
	t_job	*job;
	t_job	*next;

	if (!(job = g_shell.jobs))
		return ;
	update_status();
	job = g_shell.jobs;
	while (job)
	{
		next = job->next;
		if (job->bg && job_is_done(job))
		{
			printf("%d done.\n", job->pgid);
			remove_job_from_list(job->pgid);
			process_del(&job->processes);
			free(job);
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
	remove_job_from_list(job->pgid);
	process_del(&job->processes);
	free(job);
}

void	put_job_fg(t_job *job, bool cont)
{
	tcsetpgrp(STDIN_FILENO, job->pgid);
	if (cont)
	{
		job->notified = 0;
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
	g_rl_prompt_cr = false;//
	if (cont)
	{
		job->notified = 0;
		kill(-job->pgid, SIGCONT);
	}
}