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

int			set_pipe(t_process *process)
{
	int		fd[2];

	if ((pipe(fd)) == -1)
		return (-1); //pipe_error()
	process->stdout = fd[1];
	process->next->stdin = fd[0];
	return (0);
}

int			wait_for_job(t_job *job)
{
	int		status;

	waitpid(job->pgid, &status, 0);
	//wait(&status);
	return (WEXITSTATUS(status));
}

int			wait_for_pipeline(t_pipeline *pipeline)
{
	t_process	*process;
	int			status;

	status = 0;
	process = pipeline->processes;
	while (process)
	{
		waitpid(process->pid, &status, 0);
		process = process->next;
	}
	return (WEXITSTATUS(status));
}

int			exec_builtin(t_process *process, char **env)
{
	(void)env;
	if (set_redir(process, true) == 0)
	{
		//if (ft_strequ(argv[0], "env"))
		//	builtin_env(argv, env);
		if (ft_strequ(process->argv[0], "exit"))
			builtin_exit(process->argv);
		else if (ft_strequ(process->argv[0], "unsetenv"))
			builtin_unsetenv(process->argv);
		else if (ft_strequ(process->argv[0], "setenv"))
			builtin_setenv(process->argv);
		else if (ft_strequ(process->argv[0], "echo"))
			builtin_echo(process->argv);
		else if (ft_strequ(process->argv[0], "cd"))
			builtin_cd(process->argv);
	}
	restore_fds();
	return (0);
}

/*
** execve
*/

int			launch_process(t_process *process, pid_t pgid, bool bg)
{
	process->pid = getpid();
	process->argv = get_argv(process);
	if (!(process->path = get_exec_path(process->argv[0])))
	{
		command_not_found(process->argv[0]);
		exit(1);
	}
	else if (is_builtin(process->argv[0]))
	{
		exec_builtin(process, g_env->env);
		exit(1);
	}
	else
	{
		set_redir(process, false);
		execve(process->path, process->argv, g_env->env);
		exit(0); //wtf doc exit(1);
	}
}

/*
** loop through the processes:
** -we fork the process
** -we call launch_process()
** we wait for pipeline
*/

int			launch_pipeline(t_pipeline *pipeline, pid_t pgid, bool bg)
{
	t_process	*process;
	pid_t		pid;

	process = pipeline->processes;
	while (process)
	{
		if (process->next && set_pipe(process) != 0)
			exit(-1); //pipe_error()
		else if ((pid = fork()) == -1)
			exit(-1); //fork_error()
		else if (pid == 0)
		{
			launch_process(process, pgid, bg);
		}
		else
		{
			process->pid = pid;
			if (process->stdin != STDIN_FILENO)
				close(process->stdin);
			if (process->stdout != STDOUT_FILENO)
				close(process->stdout);
		}
		process = process->next;
	}
	/* wait for all processes to report */
	return (wait_for_pipeline(pipeline));
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
			return (launch_pipeline(pipeline, job->pgid, job->bg));
		else if (pipeline->sep->type == AND_IF
		&& launch_pipeline(pipeline, job->pgid, job->bg) != 0)
			pipeline = pipeline->next;
		else if (pipeline->sep->type == OR_IF
		&& launch_pipeline(pipeline, job->pgid, job->bg) == 0)
			pipeline = pipeline->next;
		pipeline = pipeline->next;
	}
	return (0);
}

/*
** loop through the jobs:
** -we fork the job
** -we call launch_job
** -we wait for the job or not
*/

int			launch_complete_command(t_complete_command *command)
{
	t_job	*job;
	pid_t	pid;

	job = command->jobs;
	while (job)
	{
		if ((pid = fork()) == -1)
			exit(1); //fork_error
		else if (pid == 0)
		{
			launch_job(job); //get ret and set global
			exit(0);
		}
		else
		{
			add_job(job);
			if (!job->bg)
				wait_for_job(job);
		}
		job = job->next;
	}
	return (0);
}