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

typedef struct			s_process
{
	struct s_process	*next;
	t_command			*command;
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
	struct s_job		*next;
	t_command			*command;
	t_process			*processes;
	pid_t				pgid;
	int					stdin;
	int					stdout;
	struct termios		tmodes;
	bool				has_tmodes;
	bool				notified;
	bool				bg;
	int					id;
}						t_job;

/*
** A stack in which the first element is always the last job stopped
** while it was in the foreground or started in the background
*/

t_list_head				*g_curr_job;

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
void					notif_jobs(void);

/*
** job set status
*/

void					update_status(void);
int						mark_status(pid_t pid, int status);
void					set_process_status(t_process *process, int status);
void					mark_job_as_running (t_job *job);

/*
** job get status
*/

bool					job_is_done(t_job *job);
bool					job_is_stopped(t_job *job);

/*
** current job stack related
*/

bool					is_current_job(t_job *job);
bool					is_previous_job(t_job *job);
t_list_head				*get_job_current_list_elem(t_job *job);
void					remove_current_list_elem(t_list_head *elem);
void					update_curr_job(t_job *job);

/*
** job utils
*/

t_job					*get_job(pid_t pgid);
void					add_job(t_job *job);
void					add_process(t_process *process);
void					remove_job_from_list(pid_t pgid);

/*
** job display
*/

char					*get_job_format(t_job *job);
char					*get_process_format(t_process *process);
t_dstr					*format_command(t_dstr *buf, t_command *command);
void					format_simple_command(t_dstr *buf,
						t_simple_cmd *command);
void					print_job(t_job *job, bool print_command);
void					print_job_long(t_job *job);

/*
** job new/del
*/

t_process				*process_new(t_command *cmd, int stdin, int stdout);
t_job					*job_new(t_command *cmd, int stdin, int stdout);
void					process_del(t_process **process);
void					job_del(t_job **job);

#endif
