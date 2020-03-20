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
** pipeline			: simple_command PIPE linebreak pipeline
** 					| simple_command
**
** returns a t_pipeline containing a list of processes
** we don t need to store the pipes
*/

t_pipeline		*pipe_sequence(void)
{
	t_pipeline	*pipe_seq;
	t_process	*list;
	t_process	*proc;

	if (g_parser.error || !(proc = simple_command()))
		return (NULL);
	list = proc;
	while (g_parser.token
	&& (g_parser.token->type == PIPE))
	{
		token_del(&g_parser.token);
		g_parser.token = get_next_token();
		linebreak(PIPE);
		if (!(proc->next = simple_command()))
		{
			g_parser.error = g_parser.error ? g_parser.error : NO_CMD_AFTER_PIPE;
			free_processes(&list);
			return (NULL);
		}
		proc = proc->next;
	}
	pipe_seq = (t_pipeline *)ft_xmalloc(sizeof(t_pipeline));
	pipe_seq->processes = list;
	return (pipe_seq);
}
