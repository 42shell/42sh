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
** and_or			: pipeline AND_IF linebreak and_or
**					| pipeline OR_IF linebreak and_or
**					| pipeline
** 
** returns a list of pipelines
*/

static t_pipeline	*ps_and_or(void)
{
	t_pipeline	*and_or;

	if (g_parser.error
	|| !(and_or = ps_pipeline()))
		return (NULL);
	while ((g_parser.token->type == AND_IF || g_parser.token->type == OR_IF))
	{
		and_or->sep = g_parser.token;
		g_parser.token = get_next_token();
		ps_linebreak(and_or->sep->type);
		if (!(and_or->next = ps_and_or()))
		{
			g_parser.error = g_parser.error ? g_parser.error : NO_CMD_AFTER_PIPE;
			pipeline_del(&and_or);
			return (NULL);
		}
	}
	return (and_or);
}

t_job				*ps_job()
{
	t_job			*job;
	t_pipeline		*pipelines;

	if (!(pipelines = ps_and_or()))
		return (NULL);
	job = (t_job *)ft_xmalloc(sizeof(t_job));
	job->pipelines = pipelines;
	return (job);
}