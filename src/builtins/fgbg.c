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

static int			continue_and_notif_job(t_job *job, bool bg)
{
	t_dstr		*command_format;

	command_format = ft_dstr_new(32);
	format_command(command_format, job->command);
	ft_printf("%s%s\n", command_format->str, bg ? " &" : "");
	ft_dstr_del(&command_format);
	continue_job(job, bg);
	return (0);
}

static int			fgbg_internal(char **argv, bool bg)
{
	t_list_head	*list;
	t_list_head	*curr;

	if (!g_job_control_enabled)
		return (2);
	update_status();
	list = get_fgbg_jobs_list(argv + 1);
	curr = list->next;
	while (curr && curr != list)
	{
		continue_and_notif_job((t_job *)curr->data, bg);
		update_status();
		curr = curr->next;
	}
	if (g_jobspec_error)
	{
		ft_dprintf(2, "42sh: %s: %s: No such job\n",
		bg ? "bg" : "fg", g_jobspec_error);
		ft_memdel((void **)&g_jobspec_error);
		return (1);
	}
	while (list->next != list)
		ft_list_del(list->next);
	free(list);
	return (0);
}

int					builtin_bg(char **argv, __attribute__((unused))
								t_array *env)
{
	return (fgbg_internal(argv, true));
}

int					builtin_fg(char **argv, __attribute__((unused))
								t_array *env)
{
	return (fgbg_internal(argv, false));
}