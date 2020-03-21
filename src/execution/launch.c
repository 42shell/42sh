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

int			exec_builtin(char **argv, char **env)
{
	(void)env;
	//if (ft_strequ(argv[0], "env"))
	//	builtin_env(argv, env);
	if (ft_strequ(argv[0], "exit"))
		builtin_exit(argv);
	else if (ft_strequ(argv[0], "unsetenv"))
		builtin_unsetenv(argv);
	else if (ft_strequ(argv[0], "setenv"))
		builtin_setenv(argv);
	else if (ft_strequ(argv[0], "echo"))
		builtin_echo(argv);
	else if (ft_strequ(argv[0], "cd"))
		builtin_cd(argv);
	restore_fds();
	return (0); //errors ?
}

int			exec_binary(char **argv, char **env)
{
	char	*path;

	if (!(path = get_exec_path(argv[0])))
	{
		ft_dprintf(2, "42sh: %s: command not found\n", argv[0]);
		return (1); //code?
	}
	else if (execve(path, argv, env) == -1)
	{
		ft_dprintf(2, "42sh: %s: cannot execute command\n", argv[0]);
		free(path);
		return (1); //code ?
	}
	free(path);
	return (0);
}

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
		set_redir(process, true); //always backup and restore
		exit(exec_builtin(process->argv, g_env->env));
	}
	return (0);
}

int			launch_process(t_process *process, bool bg)
{
	//expand(process);
	process->argv = get_argv(process);
	if (is_builtin(process->argv[0]))
		return (launch_builtin(process, bg));
	else if ((process->pid = fork()) == -1)
	{
		ft_dprintf(2, "42sh: too many processes\n");
		kill(STDIN_FILENO, SIGABRT);
	}
	else if (process->pid == 0)
	{
		process->pid = getpid();
		set_redir(process, true); //always backup and restore
		exit(exec_binary(process->argv, g_env->env));//?
	}
	return (0);
}

/*
** loop through the processes:
** -we fork the process
** -we call launch_process()
** we wait for pipeline
*/

int			launch_pipeline(t_pipeline *pipeline, bool bg)
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
			launch_process(process, bg);
			close_pipe(process);
		}
		else
			launch_process(process, bg);
		process = process->next;
	}
	if (!bg)
		wait_for_pipeline(pipeline);
	return (0);
}

/*
** loop through the pipelines:
** -we call launch pipeline
** -we handle the logic
** -no fork -_- stupid useless builtins
** 		if the job is launched in bg, we ll track the pipeline via job control,
**		and exec the next one if status is good
*/

int			launch_job(t_job *job)
{
	t_pipeline	*pipeline;

	pipeline = job->pipelines;
	while (pipeline)
	{
		if (job->bg || !pipeline->next)
			return (launch_pipeline(pipeline, job->bg));
		else if (pipeline->sep == AND_IF
		&& launch_pipeline(pipeline, job->bg) != 0)
			pipeline = pipeline->next;
		else if (pipeline->sep == OR_IF
		&& launch_pipeline(pipeline, job->bg) == 0)
			pipeline = pipeline->next;
		pipeline = pipeline->next;
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
		launch_job(job);
		if (job->bg)
			add_job(job);
		job = job->next;
	}
	return (g_last_exit_st);
}