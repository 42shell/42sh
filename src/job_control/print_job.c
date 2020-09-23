/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_job.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 16:46:58 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** field width of the pid + status field
*/

#define FIELD_WIDTH	38

/*
** padding to fill job_id field ( [#]+ ) when printing processes in long format
*/

int			g_padding_left;

void		set_padding_left(char *job_str)
{
	while (*job_str++ != ']')
		g_padding_left++;
	g_padding_left += 2;
}

/*
** padding to fill the space after process/job status field
** and align commands.
*/

void		pad_right(t_dstr *dstr, int n)
{
	if (n < 0)
		n = 1;
	while (n--)
		ft_dstr_add(dstr, ' ');
}

void		format_processes(t_dstr *buf, t_process *list, bool l_opt)
{
	int		start_of_line;

	while (list->next)
		list = list->next;
	while (list)
	{
		if (l_opt)
		{
			ft_dstr_add(buf, '\n');
			start_of_line = buf->len;
			format_process_info(buf, list, g_padding_left);
			pad_right(buf, FIELD_WIDTH - 2 - (buf->len - start_of_line));
			if (list->stdin == 0)
				ft_dstr_cat(buf, "  ");
		}
		ft_dstr_cat(buf, list->command_str->str);
		if (!l_opt && list->prev)
			ft_dstr_add(buf, ' ');
		list = list->prev;
	}
}

void		print_job(t_job *job, bool l_opt)
{
	t_dstr	*job_format;

	if (!job)
		return ;
	g_padding_left = 1;
	job_format = ft_dstr_new(64);
	format_job_info(job_format, job, l_opt);
	set_padding_left(job_format->str);
	if (!l_opt)
		pad_right(job_format, FIELD_WIDTH - job_format->len);
	format_processes(job_format, job->processes, l_opt);
	if (job->bg && job_is_running(job))
		ft_dstr_cat(job_format, " &");
	ft_printf("%s\n", job_format->str);
	ft_dstr_del(&job_format);
}
