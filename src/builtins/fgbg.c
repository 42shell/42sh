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

static int	notif_and_continue_job(t_job *job, bool bg)
{
	t_dstr		*command_format;

	command_format = ft_dstr_new(32);
	format_command(command_format, job->command);
	ft_printf("%s%s\n", command_format->str, bg ? " &" : "");
	ft_dstr_del(&command_format);
	continue_job(job, bg);
	return (0);
}

/*
** argv = argv + 1
*/

static int	fgbg_internal(char **argv, bool bg)
{
	t_job	*job;

	if (!*argv)
	{
		if (!g_curr_job || g_curr_job->next == g_curr_job)
		{
			ft_dprintf(2, "42sh: %s: %s: No such job\n",
			bg ? "bg" : "fg", "current");
			return (2);
		}
		job = (t_job *)g_curr_job->next->data;
	}
	else if (!(job = get_job_by_str(*argv)))
	{
		ft_dprintf(2, "42sh: %s: %s: No such job\n",
		bg ? "bg" : "fg", *argv);
		return (2);
	}
	notif_and_continue_job(job, bg);
	return (0);
}

int			builtin_bg(char **argv, __attribute__((unused)) t_array *env)
{
	if (!g_job_control_enabled)
		return (2);
	update_status();
	return (fgbg_internal(argv + 1, true));
}

int			builtin_fg(char **argv, __attribute__((unused)) t_array *env)
{
	if (!g_job_control_enabled)
		return (2);
	update_status();
	return (fgbg_internal(argv + 1, false));
}