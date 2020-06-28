/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 13:50:00 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/17 05:04:32 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
#define JOBS_L	1
#define JOBS_N	2
#define JOBS_P	4
#define JOBS_R	8
#define JOBS_S	16

bool		g_jobspec_error;

static void	set_option(int *options, char c)
{
	if (c == 'l')
		*options = ((*options | JOBS_L) & ~JOBS_P);
	else if (c == 'n')
		*options = ((*options | JOBS_N) & ~(JOBS_R | JOBS_S));
	else if (c == 'p')
		*options = ((*options | JOBS_P) & ~JOBS_L);
	else if (c == 'r')
		*options = ((*options | JOBS_R) & ~(JOBS_N | JOBS_S));
	else
		*options = ((*options | JOBS_S) & ~(JOBS_R | JOBS_N));
}

static int	get_jobs_options(char **argv, int *options)
{
	int		argc;
	int		ret;
	char	c;

	argc = 0;
	ret = 0;
	while (argv[argc])
		argc++;
	while ((c = get_opt(argc, (unsigned char **)argv)) != -1)
	{
		if (ret == 0
				&& (c == 'l' || c == 'n' || c == 'p' || c == 'r' || c == 's'))
			set_option(options, c);
		else if (ret == 0)
		{
			ft_dprintf(2, "jobs: illegal option -- %d\n", c);
			ret = -1;
		}
	}
	return (ret);
}

static int	print_one_job(t_job *job, int options)
{
	if (((options & JOBS_N) && job->notified)
	|| ((options & JOBS_R) && (job_is_stopped(job) || job_is_done(job)))
	|| ((options & JOBS_S) && !job_is_stopped(job)))
		return (0);
	if (options & JOBS_P)
		ft_dprintf(2, "%d\n", job->pgid);
	else if (options & JOBS_L)
		print_job_long(job);
	else
		print_job(job, true);
	job->notified = true;
	return (0);
}


//argv = argv + number of option arguments


static int	print_jobs(char **argv, int options)
{
	t_job	*job;

	if (!*argv)
	{
		job = g_shell.jobs->next;
		while (job)
		{
			if (job->bg || job_is_stopped(job))
				print_one_job(job, options);
			job = job->next;
		}
		return (0);
	}
	while (*argv)
	{
		if (!(job = get_job_by_str(*argv)))
		{
			g_jobspec_error = true;
			ft_dprintf(2, "42sh: jobs: %s: No such job\n", *argv);
		}
		else
			print_one_job(job, options);
		argv++;
	}
	return (0);
}

int			builtin_jobs(char **argv, __attribute__((unused)) t_array *env)
{
	int			options;

	options = 0;
	if (get_jobs_options(argv, &options) == -1)
		return (2);
	update_status();
	argv++;
	while (*argv && **argv == '-')
		argv++;
	print_jobs(argv, options);
	if (g_jobspec_error)
	{
		g_jobspec_error = 0;
		return (2);
	}
	return (0);
}
*/
