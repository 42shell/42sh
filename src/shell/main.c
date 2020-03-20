/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 19:37:33 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/23 10:55:57 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			launch_process(t_process *process)
{
	/*
	** execve
	*/
	(void)process;
	return (0);
}

int			launch_pipeline(t_pipeline *pipeline)
{
	/*
	** this is the equivalent of launch_job() int the job control doc
	**
	** loop through the processes:
	** -we fork the process
	** -we call launch_process()
	** -we don t wait for processes cause it is a pipeline
	*/
	(void)pipeline;
	return (0);
}

int			launch_job(t_job *job)
{
	/*
	** loop through the pipelines:
	** -as the pipelines are executed sequentially, we should not need to fork here
	** -we call launch_pipeline() and handle the logic (&& ||)
	*/
	(void)job;
	return (0);
}

int			exec_complete_command(t_complete_command *command)
{
	/*
	** loop through the jobs:
	** -we fork the job, we need it cause the jobs are not executed sequentially
	**  in case of background job
	** -we add the job to g_shell.jobs
	** -we call launch_job()
	** -we put the job in foreground or not (wait for it to report or not)
	*/
	(void)command;
	return (0);
}

static void	add_command(t_complete_command *command)
{
	t_complete_command	*ptr;

	if (!g_shell.commands)
		g_shell.commands = command;
	else
	{
		ptr = g_shell.commands;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = command;
	}
}

int			main_loop()
{
	t_complete_command	*command;

	while (1) //while !g_shell.quit...
	{
		g_parser.error = NOERR;
		g_shell.get_input(PS1);
		if ((g_parser.token = get_next_token())
		&& (command = complete_command()))
		{
			//exec_complete_command(complete_command)
			print_jobs(command->jobs);
			free_jobs(&command->jobs);
			free(command);
			if (g_shell.interactive_mode)
			{
				g_lexer.line[ft_strlen(g_lexer.line) - 1] = 0;
				rl_add_history(g_lexer.line);
			}
		}
		reset_lexer();
	}
	return (0);
}

int			main(int argc, char **argv)
{
	init(argc - 1, argv + 1);
	main_loop();
	return (0);
}
