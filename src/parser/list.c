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
** the following rules should be enough to get a fine shell scripting:
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
** list				: job
** 					| job separator_op list
**
** returns a list of jobs.
**
** -a newline_list() call is necessary to avoid parse errors
**  in case of empty command "\n" or ending separators "ls;".
**  Normally it should be in compound_command rule
*/

static t_job	*get_jobs(void)
{
	t_job		*jobs;
	t_token		*sep;

	ps_newline_list();
	if (!g_parser.token
	|| !(jobs = ps_job()))
		return (NULL);
	else if ((sep = ps_separator_op()))
	{
		jobs->bg = (sep->type == AMPERSAND);
		token_del(&sep);
		jobs->next = get_jobs();
		if (g_parser.error)
		{
			job_del(&jobs);
			return (NULL);
		}
	}
	return (jobs);
}

/*
** returns t_list containing a list of t_jobs
** get heredocs and handle parsing errors
*/

t_list			*ps_list(void)
{
	t_list		*list;
	t_job		*jobs;

	if ((jobs = get_jobs())
	&& ps_get_all_heredocs() == NOERR)
	{
		list = (t_list *)ft_xmalloc(sizeof(t_list));
		list->jobs = jobs;
		return (list);
	}
	else if (g_parser.error)
	{
		ps_error(g_parser.token ? g_parser.token->value->str : "(null)");
		token_del(&g_parser.token);
	}
	return (NULL);
}