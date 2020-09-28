/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_control.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 12:28:09 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 17:18:04 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOB_CONTROL_H
# define JOB_CONTROL_H

# include "shell.h"

# ifdef HIDE_PID
#  define SHOW_PID		0
# else
#  define SHOW_PID		1
# endif

# ifdef HIDE_NOTIF
#  define SHOW_NOTIF	0
# else
#  define SHOW_NOTIF	1
# endif

typedef struct			s_process
{
	struct s_job		*job;
	struct s_process	*next;
	struct s_process	*prev;
	t_command			*command;
	t_dstr				*command_str;
	pid_t				pid;
	int					stdin;
	int					stdout;
	int					status;
	int					signaled;
	bool				stopped;
	bool				done;
}						t_process;

typedef struct			s_job
{
	int					id;
	struct s_job		*next;
	struct s_job		*prev;
	t_command			*command;
	t_process			*processes;
	pid_t				pgid;
	int					stdin;
	int					stdout;
	struct termios		tmodes;
	bool				has_tmodes;
	bool				notified;
	bool				invert_ret;
	bool				bg;
	t_array				*fds_to_close;
}						t_job;

extern t_job			*g_current_jobs;
extern t_job			*g_jobs;
extern int				g_greatest_job_id;

/*
** launch job/process
*/

int						launch_job(t_job *job);
int						launch_process(t_process *process, int fd_to_close);

/*
** job control
*/

void					put_job_fg(t_job *job, bool cont);
void					put_job_bg(t_job *job, bool cont);
void					wait_for_job(t_job *job);
void					continue_job(t_job *job, bool bg);
int						set_process_status(pid_t pid, int status);
void					update_status(void);
void					update_jobs(void);

/*
** job status
*/

bool					job_is_done(t_job *job);
bool					job_is_stopped(t_job *job);
bool					job_is_running(t_job *job);

/*
** job list
*/

t_list_head				*get_sorted_jobs_list(void);
void					move_job_in_persistent_list(t_job *job);
void					add_job_to_list(t_job **head, t_job *job, bool set_id);
void					remove_job_from_list(t_job **head, t_job *job);
void					del_job_from_list(t_job **head, t_job *job);

/*
** job utils
*/

bool					job_is_in_list(t_job *list, t_job *job);
bool					is_last_job(t_job *job);
bool					is_before_last_job(t_job *job);
int						cmp_job_id(void *a, void *b);
void					update_jobs_greatest_id(void);

/*
** process utils
*/

void					add_process_to_job(t_job *job, t_process *process);
t_process				*get_process(pid_t pid);
void					reset_signals(void);

/*
** job display
*/

void					format_job_status(t_dstr *buf, t_job *job);
void					format_job_id(t_dstr *buf, t_job *job);
void					format_exit_status(t_dstr *buf, t_process *process);
void					format_process_info(t_dstr *buf, t_process *process,
						int padding);
void					format_processes(t_dstr *buf, t_process *list,
						bool l_opt);
void					print_job(t_job *job, bool l_opt);

/*
** job new/del
*/

t_process				*process_new(t_command *cmd, int stdin, int stdout);
t_job					*job_new(t_command *cmd, int stdin, int stdout);
void					process_list_del(t_process **process_list);

#endif
