/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fgbg.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 13:50:00 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/11 13:43:37 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static t_job		*get_job_by_str(char *str)
{
	t_job	*job;
	int		id;

	if (str[0] == '%')
	{
		if (str[1] == '+')
			return (g_shell.curr_job);
		else if (str[1] == '-')
			return (g_shell.prev_job);
		id = ft_atoi(++str) - 1;
	}
	else
		id = ft_atoi(str) - 1;
	job = g_shell.jobs->next;
	while (job && job->id != id)
		job = job->next;
	return (job);
}

static t_list_head	*get_jobs_list(char **argv)
{
	t_list_head	*list;
	t_job		*job;
	int			i;

	i = 1;
	list = ft_list_first_head(NULL);
	if (!argv[i])
	{
		ft_list_add(g_shell.curr_job, list);
		return (list);
	}
	while (argv[i])
	{
		if (!(job = get_job_by_str(argv[i])))
		{
			while (list->next != list)
				ft_list_del(list->next);
			free(list);
			return (NULL);
		}
		ft_list_add(job, list);
		i++;
	}
	return (list);
}

int					builtin_bg(char **argv)
{
	t_list_head	*list;
	t_list_head	*curr;
	t_job		*job;
	t_dstr		*buf;

	if (!g_shell.jobs->next || !(list = get_jobs_list(argv)))
	{
		ft_dprintf(2, "42sh: bg: %s: No such job\n",
		argv[1] ? argv[1] : "current");
		return (1);
	}
	curr = list->next;
	while (curr != list)
	{
		job = (t_job *)curr->data;
		buf = ft_dstr_new(32);
		format_command(job->command, buf);
		ft_printf("%s &\n", buf->str);
		continue_job(job, true);
		ft_dstr_del(&buf);
		curr = curr->next;
	}
	while (list->next != list)
		ft_list_del(list->next);
	free(list);
	return (0);
}

int					builtin_fg(char **argv)
{
	t_list_head	*list;
	t_list_head	*curr;
	t_job		*job;
	t_dstr		*buf;

	if (!g_shell.jobs->next || !(list = get_jobs_list(argv)))
	{
		ft_dprintf(2, "42sh: fg: %s: No such job\n",
		argv[1] ? argv[1] : "current");
		return (1);
	}
	curr = list->next;
	while (curr != list)
	{
		job = (t_job *)curr->data;
		buf = ft_dstr_new(32);
		format_command(job->command, buf);
		ft_printf("%s\n", buf->str);
		continue_job(job, false);
		ft_dstr_del(&buf);
		curr = curr->next;
	}
	while (list->next != list)
		ft_list_del(list->next);
	free(list);
	return (0);
}