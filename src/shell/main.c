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

int			main_loop()
{
	t_job	*job;

	while (1) //while !g_shell.quit...
	{
		g_parser.error = NOERR;
		if (!g_lexer.line || !g_lexer.line[g_lexer.i])
			g_shell.get_input(PS1);
		if ((g_shell.jobs = complete_command()))
		{
			job = g_shell.jobs;
			while (job)
			{
				launch_job(job);
				job = job->next;
			}
			job_del(&g_shell.jobs);
		}
		if (g_shell.interactive_mode)
		{
			if (g_lexer.line)
			{
				g_lexer.line[ft_strlen(g_lexer.line) - 1] = 0;
				rl_add_history(g_lexer.line);
			}
			reset_lexer();
		}
	}
	return (0);
}

int			main(int argc, char **argv)
{
	init(argc - 1, argv + 1);
	main_loop();
	return (0);
}
