/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 04:37:16 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/19 17:21:29 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** pipeline			: process PIPE linebreak pipeline
**					: process
**
** returns a list of t_process
*/

static t_process	*get_processes(void)
{
	t_process		*processes;

	if (g_parser.error
	|| !(processes = ps_process()))
		return (NULL);
	while (g_parser.token->type == PIPE)
	{
		token_del(&g_parser.token);
		g_parser.token = get_next_token();
		ps_linebreak(PIPE);
		if (!(processes->next = get_processes()))
		{
			g_parser.error = g_parser.error ? g_parser.error : NO_CMD_AFTER_PIPE;
			process_del(&processes);
			return (NULL);
		}
	}
	return (processes);
}

/*
** pipeline			: pipe_sequence
**
** returns a t_pipeline containing a list of processes
*/

t_pipeline			*ps_pipeline(void)
{
	t_pipeline		*pipeline;
	t_process		*processes;

	if (!(processes = get_processes()))
		return (NULL);
	pipeline = (t_pipeline *)ft_xmalloc(sizeof(t_pipeline));
	pipeline->processes = processes;
	return (pipeline);
}
