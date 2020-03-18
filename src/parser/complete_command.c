/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complete_command.c                                 :+:      :+:    :+:   */
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

/*
list             : and_or
                 | and_or separator_op list
*/

t_job		*list(void)
{
	t_job	*job;
	t_node	*ast;

	job = NULL;
	if (g_parser.error || !g_parser.token)
		return (NULL);
	if ((ast = and_or(NULL)))
	{
		job = job_new(ast);
		if ((job->sep = separator_op())
		&& g_parser.token && g_parser.token->type != NEWLINE
		&& (job->next = list()))
			job->next->prev = job;
	}
	get_all_heredocs();
	return (job);
}

/*

this rule seems to be only useful in batch mode (or maybe in builtins like function definition ??)

need rule with EMPTY somewhere to avoid parse errors for empty lines "\n"

complete_command : list separator 
                 | list
*/

t_job		*complete_command(void)
{
	t_job	*jobs;

	jobs = NULL;
	if (g_parser.error)
		return (NULL);
	else if (!g_parser.token)
		g_parser.token = get_next_token();
	jobs = list();
	separator();
	ft_memdel((void **)&g_parser.heredocs);
	if (g_parser.error && g_parser.error != HEREDOC_NO_DELIM)
		job_del(&jobs);
	return (jobs);
}
