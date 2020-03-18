/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 08:48:18 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/14 17:31:49 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			job_del(t_job **jobs)
{
	if (!jobs || !*jobs)
		return (0);
	free_ast_nodes((*jobs)->ast, false);
	ft_memdel((void **)jobs);
	return (0);
}

t_job		*job_new(t_node *ast)
{
	t_job	*job;

	job = (t_job *)ft_xmalloc(sizeof(t_job));
	job->ast = ast;
	return (job);
}

void		free_ast_nodes(t_node *node, bool par_is_pattern)
{
	int		i;
	//bool	cur_is_pattern;

	(void)par_is_pattern;
	//cur_is_pattern = false;
	if (node == NULL)
		return ;
	else if (node->type == NODE_PROCESS)
		process_del((t_process **)&node->data);
	else if (node->data != NULL /*&& !par_is_pattern*/)
	{
		token_del((t_token **)&node->data);
		//cur_is_pattern = (node_token(node)->type == PATTERN);
	}
	//else if (par_is_pattern)
	//	array_destroy(node->data);
	i = 0;
	while (i < node->nb_children)
		free_ast_nodes(node->child[i++], /*cur_is_pattern*/ 0);
	free(node->child);
	free(node);
}
