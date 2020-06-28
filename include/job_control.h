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

extern t_job			*g_current_jobs;
extern t_job			*g_jobs;

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
void					update_status(void);
void					continue_job(t_job *job, bool bg);
void					update_jobs(bool notif);


/*
** utils
*/

bool					job_is_done(t_job *job);
bool					job_is_stopped(t_job *job);
t_job					*job_dup(t_job *job);
t_process				*process_list_dup(t_process *process_list);
t_process				*get_process(pid_t pid);
t_job					*get_job_by_str(char *str);

/*
** jobs lists
*/

void					add_job_to_list(t_job **head, t_job *job, bool set_id);
void					add_process_to_job(t_job *job, t_process *process);
void					remove_job_from_list(t_job **head, t_job *job);
void					remove_command_from_list(t_command **head, t_command *command);
void					add_command_to_list(t_command **head, t_command *command);

/*
** current job stack related
*/

/*
bool					is_current_job(t_job *job);
bool					is_previous_job(t_job *job);
t_list_head				*get_job_current_list_elem(t_job *job);
void					remove_current_list_elem(t_list_head *elem);
void					update_curr_job(t_job *job);
*/

/*
** job display
*/

char					*get_job_format(t_job *job);
char					*get_process_format(t_process *process);
void					format_command(t_dstr *buf, t_command *command);
void					print_job(t_job *job, bool print_command);
void					print_job_long(t_job *job);

/*
** job new/del
*/

t_process				*process_new(t_command *cmd, int stdin, int stdout);
t_job					*job_new(t_command *cmd, int stdin, int stdout);
void					process_list_del(t_process **process_list);

#endif
