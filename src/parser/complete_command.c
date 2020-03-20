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
** -returns a list of t_job
** -the separator operator is stored in the pipe struct
** -again a newline_list() call is necessary.
**  As compound_command is not implemented, if the token after the separator is a newline,
**  it will not be skipped in next and_or() call. In simple_command(), we trigger an error
**  at the very beginning only if there is a token, and its not a valid argv (if there is
**  no token, it s not an error its just an empty line).
**  NEWLINE is not valid but we dont want an error so we skip it here.
*/

static t_job		*list(void)
{
	t_job	*jobs;
	t_job	*job;

	if (!(job = and_or()))
		return (NULL);
	jobs = job;
	while (job && (job->sep = separator_op()))
	{
		newline_list();
		job->next = and_or();
		if (g_parser.error)
		{
			free_jobs(&jobs);
			return (NULL);
		}
		job = job->next;
	}
	if (get_all_heredocs() != NOERR)
		free_jobs(&jobs);
	return (jobs);
}

/*
** complete_command : list separator 
** 					| list
** 
** -get the first token
** -call newline_list to skip the newlines (this allows us to return without error
**  in case of an empty command ("\n")). According to the grammar, this happens in
**  compound_command rule, which is not implemented yet, this is why whis call
**  is neccessary.
** -handle parse errors and returns NULL if something went wrong.
** -skip the following separator op and newline if any.
** -returns a t_complete_command containing a list of jobs.
*/

/*
** -the call to newline_list should not be here, but in the grammar,
** 	the first call to newline_list is in compound_command(). So i m forced to
**  call it here for the moment, to skip the first token if it is a newline.
*/

t_complete_command	*complete_command(void)
{
	t_complete_command	*command;
	t_job				*jobs;

	g_parser.token = get_next_token();
	newline_list();
	if (!(jobs = list()))
	{
		if (g_parser.error != SILENT_ABORT)
			ft_dprintf(2, "42sh: syntax error near unexpected token '%s'\n",
			g_parser.token ? g_parser.token->value->str : "(null)");
		token_del(&g_parser.token);
		return (NULL);
	}
	separator();
	command = (t_complete_command *)ft_xmalloc(sizeof(t_complete_command));
	command->jobs = jobs;
	return (command);
}
