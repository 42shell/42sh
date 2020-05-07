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
			return ((g_curr_job && g_curr_job->next != g_curr_job) ? g_curr_job->next->data : NULL);
		else if (str[1] == '-')
		{
			if (g_curr_job && g_curr_job->next != g_curr_job)
				return (g_curr_job->next->next != g_curr_job
				? g_curr_job->next->next->data : g_curr_job->next->data);
			return (NULL);
		}
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
		if (g_curr_job && g_curr_job->next != g_curr_job)
		{
			ft_list_add(g_curr_job->next->data, list);
			return (list);
		}
		free(list);
		return (NULL);
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
		ft_list_add_tail(job, list);
		i++;
	}
	return (list);
}

int					builtin_bg(char **argv)
{
	t_list_head	*list;
	t_list_head	*curr;
	t_job		*job;
	t_dstr		*command_format;

	if (!g_job_control_enabled)
		return (2);
	update_status();
	if (!g_shell.jobs->next || !(list = get_jobs_list(argv)))
	{
		ft_dprintf(2, "42sh: bg: %s: No such job\n",
		argv[1] ? argv[1] : "current");
		return (1);
	}
	curr = list->next;
	while (curr && curr != list)
	{
		job = (t_job *)curr->data;
		command_format = ft_dstr_new(32);
		format_command(command_format, job->command);
		ft_printf("%s &\n", command_format->str);
		ft_dstr_del(&command_format);
		continue_job(job, true);
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
	t_dstr		*command_format;

	if (!g_job_control_enabled)
		return (2);
	update_status();
	if (!g_shell.jobs->next || !(list = get_jobs_list(argv)))
	{
		ft_dprintf(2, "42sh: fg: %s: No such job\n",
		argv[1] ? argv[1] : "current");
		return (1);
	}
	curr = list->next;
	while (curr && curr != list)
	{
		job = (t_job *)curr->data;
		command_format = ft_dstr_new(32);
		format_command(command_format, job->command);
		ft_printf("%s\n", command_format->str);
		ft_dstr_del(&command_format);
		continue_job(job, false);
		curr = curr->next;
	}
	while (list->next != list)
		ft_list_del(list->next);
	free(list);
	return (0);
}