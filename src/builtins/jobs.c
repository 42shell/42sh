/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 13:50:00 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 16:45:44 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

#define JOBS_L	1
#define JOBS_N	2
#define JOBS_P	4
#define JOBS_R	8
#define JOBS_S	16

static t_list_head	*get_jobs_list(int options)
{
	t_list_head	*list;
	t_job		*job;

	list = ft_list_first_head(NULL);
	job = g_shell.jobs->next;
	while (job)
	{
		if (((options & JOBS_N) && job->notified == false)
		|| ((options & JOBS_R) && !job_is_stopped(job) && !job_is_done(job))
		|| ((options & JOBS_S) && job_is_stopped(job))
		|| !(options & (JOBS_N | JOBS_R | JOBS_S)))
			ft_list_add(job, list);
		job = job->next;
	}
	return (list);
}

static void			set_option(int *options, char c)
{
	if (c == 'l')
		*options = ((*options | JOBS_L) &~ JOBS_P);
	else if (c == 'n')
		*options = ((*options | JOBS_N) &~ (JOBS_R | JOBS_S));
	else if (c == 'p')
		*options = ((*options | JOBS_P) &~ JOBS_L);
	else if (c == 'r')
		*options = ((*options | JOBS_R) &~ (JOBS_N | JOBS_S));
	else
		*options = ((*options | JOBS_S) &~ (JOBS_R | JOBS_N));
}

static int			get_jobs_options(char **argv, int *options)
{
	int		argc;
	int		ret;
	char	c;

	argc = 0;
	ret = 0;
	while (argv[argc])
		argc++;
	while ((c = get_opt(argc, argv)) != -1)
	{
		if (ret == 0 && (c == 'l' || c == 'n' || c == 'p' || c == 'r' || c == 's'))
			set_option(options, c);
		else if (ret == 0)
		{
			ft_dprintf(2, "jobs: illegal option -- %d\n", c);
			ret = -1;
		}
	}
	return (ret);
}

int				builtin_jobs(char **argv,
							__attribute__((unused)) t_array *env)
{
	t_list_head	*list;
	t_list_head	*curr;
	t_job		*job;
	int			options;

	(void)argv;
	options = 0;
	if (!g_job_control_enabled || !g_shell.jobs)
		return (0);
	if (get_jobs_options(argv, &options) == -1)
		return (2);
	update_status();
	list = get_jobs_list(options);
	curr = list->next;
	while (curr != list)
	{
		job = (t_job *)curr->data;
		if (options & JOBS_L)
			print_job_long(job);
		else if (options & JOBS_P)
			ft_printf("%d\n", job->pgid);
		else
			print_job(job, true);
		job->notified = true;
		curr = curr->next;
	}
	while (list->next != list)
		ft_list_del(list->next);
	free(list);
	return (0);
}
