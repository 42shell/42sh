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

/*
** the following rules should be enough to get a fine shell scripting:
**
** command			: simple_command
** 					| compound_command 
**
** compound_command : brace_group
** 					| subshell
**					| if_clause
**					| while_clause
** 
** and corresponding sub rules
*/

/* ************************************************************************** */

/*
** list				: and_or
** 					| and_or separator_op list 
** 
** returns a list of jobs
** the separator operator is stored in the pipe struct
*/

t_job		*list(void)
{
	t_job	*jobs;
	t_job	*job;

	if (!(job = and_or()))
		return (NULL);
	jobs = job;
	while (job && (job->sep = separator_op()))
	{
		if (!(job->next = and_or()))
			g_parser.error = g_parser.error ? g_parser.error : NO_CMD_AFTER_SEP;
		job = job->next;
	}
	get_all_heredocs();
	return (jobs);
}

/*
** complete_command : list separator 
** 					| list
** 
** -returns a list of jobs and skip the following newlines
** -handle parse errors, if a token is still available, there is an error.
*/

/*
** -the call to newline_list should not be here, but in the grammar,
** 	the first call to newline_list is in compound_command(). So i m forced to
**  call it here for the moment, to skip the first token if it is a newline.
*/

t_job		*complete_command(void)
{
	t_job	*jobs;

	if (!g_parser.token)
		g_parser.token = get_next_token();
	newline_list();
	if ((jobs = list()))
		separator();
	if (g_parser.token)
	{
		if (g_parser.error != SILENT_ABORT)
			ft_dprintf(2, "42sh: syntax error near unexpected token '%s'\n",
			g_parser.token->value->str);
		token_del(&g_parser.token);
		free_jobs(&jobs);
	}
	return (jobs);
}
