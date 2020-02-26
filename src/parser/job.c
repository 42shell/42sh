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

t_node		*parse_error(int code, char *near, t_node *to_free)
{
	g_parse_error = code;
	ft_dprintf(2, "42sh: syntax error near unexpected token '%s'\n", near);
	if (to_free)
		free_ast_nodes(to_free, false);
	return (NULL);
}

void		del_job(t_job **job)
{
	if (!job || !*job)
		return ;
	free_ast_nodes((*job)->ast, false);
	del_job(&((*job)->next));
	ft_memdel((void **)job);
	//destroy_heredocs
}

t_job		*get_job(void)
{
	t_job	*job;
	t_node	*ast;

	job = NULL;
	if (!(g_token = get_next_token()))
		return (NULL);
	if ((ast = and_or()))
	{
		job = (t_job *)ft_xmalloc(sizeof(t_job));
		job->ast = ast;
		if (g_token && (g_token->type == SEMI || g_token->type == AMPERSAND))
		{
			job->bg = (g_token->type == AMPERSAND);
			token_del(&g_token);
			job->next = get_job();
		}
	}
	return (job);
}

/*
** gets the list of ASTs for the input, then executes all of them sequentially.
** ex: the input "ls ; cat; wc" gets a list of 3 asts
** asts are trees containing t_tokens in their "data" fields.
** one node can have an infinite number of children.
*/

/*
int				parse(void)
{
	t_ast	*ast;
	t_ast	*ptr;

	g_parse_error = NOERR;
	if ((ast = get_ast()) && !g_parse_error)
	{
		//run();
		ptr = ast;
		while (ptr)
		{
			print_ast(ptr->node, 0);//run(ast, env);
			printf("\n");
			ptr = ptr->next;
		}
	}
	del_ast(&ast);
	return (0);
}
*/
