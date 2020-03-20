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
** pipe_sequence	: simple_command PIPE linebreak pipe_sequence
**					: simple_command
**
** returns a list of t_process
*/

static t_process	*ps_pipe_sequence(void)
{
	t_process		*pipe_sequence;

	if (g_parser.error
	|| !(pipe_sequence = ps_simple_command()))
		return (NULL);
	while (g_parser.token->type == PIPE)
	{
		token_del(&g_parser.token);
		g_parser.token = get_next_token();
		ps_linebreak(PIPE);
		if (!(pipe_sequence->next = ps_pipe_sequence()))
		{
			g_parser.error = g_parser.error ? g_parser.error : NO_CMD_AFTER_PIPE;
			process_del(&pipe_sequence);
			return (NULL);
		}
	}
	return (pipe_sequence);
}

/*
** pipeline			: pipe_sequence
**
** returns a t_pipeline containing a pipe_sequence
** for the moment ! are not handled...
** if it s a NOT operator it will be mandatory for shell script
*/

t_pipeline			*ps_pipeline(void)
{
	t_pipeline		*pipeline;
	t_process		*processes;

	if (!(processes = ps_pipe_sequence()))
		return (NULL);
	pipeline = (t_pipeline *)ft_xmalloc(sizeof(t_pipeline));
	pipeline->processes = processes;
	return (pipeline);
}
