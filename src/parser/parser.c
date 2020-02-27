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

int			parse_error(int code, char *near)
{
	if (g_parser.error)
		return (0);
	else if (g_parser.error != SILENT_ABORT && near)
		ft_dprintf(2,
		"42sh: syntax error near unexpected token '%s'\n",
		near);
	g_parser.error = code;
	free(near);
	return (0);
}

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
t_job		*get_jobs(void)
{
	t_job	*jobs;

	if (!jobs = list();
	if (!(g_parser.token = get_next_token())
	|| !(job->ast = and_or()))
	{
		job_del(&job);
		return (NULL);
	}
	//else if (g_parser.token
	//&& (g_parser.token->type == SEMI || g_parser.token->type == AMPERSAND))
	//{
	//	job->bg = (g_parser.token->type == AMPERSAND);
	//	token_del(&g_parser.token);
	//	job->next = get_jobs();
	//}
	//if (g_parser.error)
	//{
	//	job_del(&g_shell.jobs);
	//	return (NULL);
	//}
	return (jobs);
}
*/

/*
while (g_parser.token)
{
	if (!(ast = and_or()))
		return (parse_error());
	tmp = job_new(ast);
	if (!jobs)
		jobs = tmp;
	else
		ptr->next = tmp;
	if (g_parser.token
	&& (g_parser.token->type == SEMI || g_parser.token->type == AMPERSAND))
	{
		tmp->bg = (g_parser.token->type == AMPERSAND);
		token_del(&g_parser.token);
		ptr = tmp;
	}
}
*/
