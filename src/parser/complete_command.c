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
** -returns a list of t_job, a list struct may be overkill for the moment...
** -the separator operator is stored in the pipe struct
** -a newline_list() call is necessary to avoid parse errors
**  in case of empty command "\n" or ending separators "ls;".
**  Normally it should be in compound_command rule
*/

t_job					*ps_list(void)
{
	t_job				*list;

	ps_newline_list();
	if (!g_parser.token
	|| !(list = ps_job()))
		return (NULL);
	else if ((list->sep = ps_separator_op()))
	{
		list->next = ps_list();
		if (g_parser.error)
		{
			job_del(&list);
			return (NULL);
		}
	}
	return (list);
}

/*
** complete_command : list separator 
** 					| list
** 
** -get the list of jobs and the heredocs
** -skip eventual separator and newline and returns a t_complete_command 
**  containing the list of jobs if there is one and everything went good
** -handle parse errors and returns NULL if something went wrong.
*/

t_complete_command		*ps_complete_command(void)
{
	t_complete_command	*complete_command;
	t_job				*jobs;

	if ((jobs = ps_list())
	&& ps_get_all_heredocs() == NOERR)
	{
		ps_separator();
		complete_command =
		(t_complete_command *)ft_xmalloc(sizeof(t_complete_command));
		complete_command->jobs = jobs;
		return (complete_command);
	}
	else if (g_parser.error)
	{
		ps_error(g_parser.token ? g_parser.token->value->str : "(null)");
		token_del(&g_parser.token);
	}
	return (NULL);
}
