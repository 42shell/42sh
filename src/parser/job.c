/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/13 18:32:06 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/* ************************************************************************** */

//						EXECUTION FOLDER

/*
** depth first tree traversal
** if we encounter a command or a pipe node, stop traversing and execute it.
** for now, this function actually always stops at the first call.
*/

/*
static int	run(void)
{
	t_ast	*tmp;

	while (ast != NULL)
	{
		if (g_parse_error == NOERR)
		{
			get_all_heredocs(&g_heredocs);
			if (g_parse_error != SILENT_ABORT)
			{
				execute(ast->node, env);
			}
		}
		free_ast_nodes(ast->node, false);
		tmp = ast;
		ast = ast->next;
		free(tmp);
	}
	g_heredocs.nb_children = 0;
	return (0);
}
*/
/* ************************************************************************** */


int			job_del(t_job **jobs)
{
	if (!jobs || !*jobs)
		return (0);
	free_ast_nodes((*jobs)->ast, false);
	job_del(&((*jobs)->next));
	ft_memdel((void **)jobs);
	return (0);
}

t_job		*job_new()
{
	return ((t_job *)ft_xmalloc(sizeof(t_job)));
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
	get_all_heredocs();
	if (g_parser.error)
	{
		ft_memdel((void **)&g_parser.heredocs);
		job_del(&jobs);
	}
	return (jobs);
}
