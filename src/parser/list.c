/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/13 18:32:06 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
list             : and_or
                 | and_or separator_op list
*/

t_job		*list(void)
{
	t_job	*jobs;
	t_node	*ast;

	jobs = NULL;
	if (g_parser.error || !g_parser.token)
		return (NULL);
	if ((ast = and_or()))
	{
		jobs = job_new();
		jobs->ast = ast;
		if ((jobs->sep = separator_op()))
			jobs->next = list();
	}
	if (g_parser.error)
		job_del(&jobs);
	return (jobs);
}
