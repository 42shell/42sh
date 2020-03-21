/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and_or.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/13 18:32:06 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** job				: pipeline AND_IF linebreak job
**					| pipeline OR_IF linebreak job
**					| pipeline
** 
** returns a list of t_pipeline.
*/

static t_pipeline	*get_pipelines(void)
{
	t_pipeline		*pipelines;

	if (g_parser.error
	|| !(pipelines = ps_pipeline()))
		return (NULL);
	while ((g_parser.token->type == AND_IF || g_parser.token->type == OR_IF))
	{
		pipelines->sep = g_parser.token->type;
		token_del(&g_parser.token);
		g_parser.token = get_next_token();
		ps_linebreak(pipelines->sep);
		if (!(pipelines->next = get_pipelines()))
		{
			g_parser.error = g_parser.error ? g_parser.error : NO_CMD_AFTER_PIPE;
			pipeline_del(&pipelines);
			return (NULL);
		}
	}
	return (pipelines);
}

/*
** returns t_job containing a list of pipelines (and_or).
** 
** -the separator operator is stored in the pipeline struct
*/

t_job			    *ps_job()
{
	t_job		    *job;
	t_pipeline  	*pipelines;

	if (!(pipelines = get_pipelines()))
		return (NULL);
	job = (t_job *)ft_xmalloc(sizeof(t_job));
	job->pipelines = pipelines;
	return (job);
}