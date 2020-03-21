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
** job control folder
*/
void		add_job(t_job *job)
{
	t_job	*ptr;

	if (!g_shell.jobs)
		g_shell.jobs = job;
	else
	{
		ptr = g_shell.jobs;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = job;
	}
}

/* ************************************************************************** */

/*
** execute the process
*/

int			launch_process(t_process *process)
{
	process->pid = getpid();
	process->argv = get_argv(process);
	if (is_builtin(process->argv[0]))
	{
		set_redir(process);//fix this function
	 	exec_builtin(process->argv, g_env->env);
	}
	else
	{
		set_redir(process);//fix this function
		exec_binary(process->argv, g_env->env);
	}
	exit(0);
}

/*
** loop through the processes:
** -we fork the process
** -we call launch_process()
** we wait for pipeline
*/

int			launch_pipeline(t_pipeline *pipeline)
{
	t_process	*process;
	pid_t		pid;

	process = pipeline->processes;
	while (process)
	{
		if (process->next
		&& set_pipe(process) == -1)
		{
			ft_dprintf(2, "42sh: failed to create pipe\n");
			exit(1); //pipe_error()
		}
		else if ((pid = fork()) == -1)
		{
			ft_dprintf(2, "42sh: too many processes, aborting\n");
			kill(0, SIGABRT);
		}
		else if (pid == 0)
			launch_process(process);
		process->pid = pid;
		close_pipe(process);
		process = process->next;
	}
	wait_for_pipeline(pipeline);
	return (g_last_exit_st);
}

/*
** loop through the pipelines:
** -we call launch pipeline
** -we handle the logic
*/

int			launch_job(t_job *job)
{
	t_pipeline	*pipeline;

	pipeline = job->pipelines;
	while (pipeline)
	{
		if (!pipeline->sep)
			return (launch_pipeline(pipeline));
		else if (pipeline->sep == AND_IF
		&& launch_pipeline(pipeline) != 0)
			pipeline = pipeline->next;
		else if (pipeline->sep == OR_IF
		&& launch_pipeline(pipeline) == 0)
			pipeline = pipeline->next;
		pipeline = pipeline->next;
	}
	return (g_last_exit_st);
}

/*
** loop through the jobs:
** -we fork the job
** -we call launch_job in the child
** -we wait for the job or not
*/

int					launch_list(t_list *list)
{
	t_job	*job;
	pid_t	pid;

	job = list->jobs;
	while (job)
	{
		if ((pid = fork()) == -1)
			kill_all_forks();
		else if (pid == 0)
		{
			launch_job(job);
			exit(g_last_exit_st);
		}
		if (g_last_exit_st)
			;//exec_error();
		else
		{
			add_job(job);
			if (!job->bg)
				wait_for_job(job);
		}
		job = job->next;
	}
	return (g_last_exit_st);
}