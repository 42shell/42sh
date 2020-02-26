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

static t_job	*parse_error(void)
{
	ft_dprintf(2, "42sh: syntax error near unexpected token '%s'\n",
												g_parser.error_near);
	free(g_parser.error_near);
	g_parser.error_near = NULL;
	g_parser.error = 0;
	return (NULL);
}

void			job_del(t_job **jobs)
{
	if (!jobs || !*jobs)
		return ;
	free_ast_nodes((*jobs)->ast, false);
	job_del(&((*jobs)->next));
	ft_memdel((void **)jobs);
}

t_job			*job_new(t_node *ast)
{
	t_job	*job;

	job = (t_job *)ft_xmalloc(sizeof(t_job));
	job->ast = ast;
	return (job);
}

t_job			*get_jobs(void)
{
	t_job	*job;
	t_node	*ast;

	job = NULL;
	if (!(g_parser.token = get_next_token()))
		return (NULL);
	else if (!(ast = and_or()))
		return (parse_error());
	job = job_new(ast);
	if (g_parser.token
	&& (g_parser.token->type == SEMI || g_parser.token->type == AMPERSAND))
	{
		job->bg = (g_parser.token->type == AMPERSAND);
		token_del(&g_parser.token);
		if (!(job->next = get_jobs()))
			return (parse_error());
	}
	return (job);
}
