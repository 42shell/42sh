/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/13 18:32:06 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** returns commands in this format:
**
** ls -l 2 > test -a -f
**
** t_node
** {
** 		data =		t_process
** 					{
** 						t_token **argv		= { ls, -l, -a, -f };
** 						t_redir **redirs	= { 2> };
** 					}
** 		childs = 	NULL;
** }
*/

t_node		*command(void)
{
	t_node		*command_node;
	t_process	*process;
	t_redir		*redirect;

	if (g_parser.error)
		return (NULL);
	process = process_new();
	command_node = node_new(process);
	while (!g_parser.error && g_parser.token
	&& ((redirect = io_redirect()) || g_parser.token->type == WORD))
	{
		if (redirect)
			add_process_redir(process, redirect);
		else
		{
			add_process_arg(process, g_parser.token);
			g_parser.token = get_next_token();
		}
	}
	if (g_parser.error || (!process->argv && !process->redirs))
	{
		process_del(&process);
		ft_memdel((void **)&command_node);
	}
	return (command_node);
}

/*
list             : and_or
                 | and_or separator_op list
*/

t_job		*list(void)
{
	t_job	*jobs;
	t_node	*ast;

	jobs = NULL;
	if (g_parser.error || !g_parser.token)
		return (NULL);
	if ((ast = and_or(NULL)))
	{
		jobs = job_new();
		jobs->ast = ast;
		if ((jobs->sep = separator_op()))
			jobs->next = list();
	}
	return (jobs);
}

/*
complete_command : list separator
                 | list
*/

t_job		*complete_command(void)
{
	t_job	*jobs;
	t_job	*last_job;

	jobs = NULL;
	if (g_parser.error || !g_parser.token)
		return (NULL);
	if ((jobs = list()))
	{
		separator();
		if (g_parser.token)
		{
			last_job = jobs;
			while (last_job->next)
				last_job = last_job->next;
			last_job->next = list();
		}
	}
	if (g_parser.error)
	{
		ft_memdel((void **)&g_parser.heredocs);
		job_del(&jobs);
	}
	else
		get_all_heredocs();
	return (jobs);
}
