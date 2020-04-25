/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 15:00:39 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/24 01:05:17 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "shell.h"
#include <errno.h>

# define ERROR_REDIR_OPEN	-1
# define ERROR_REDIR_BAD_FD	-2

# define EXEC_ASYNC				1			
# define EXEC_AND_OR			2
# define EXEC_PIPELINE			4
# define EXEC_ALREADY_FORKED	8

# define JOB_DONE			1		
# define JOB_STOPPED		2		

typedef struct			s_fd_backup
{
	int					backup;
	int					orig_number;
}						t_fd_backup;

typedef struct			s_process
{
	struct s_process	*next;
	t_command			*command;
	pid_t				pid;
	int					stdin;
	int					stdout;
	int					status;
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
** associative table of command names and binaries paths.
** -an entry is added when a command is executed if the command is not the table.
** -the table is (should be) flushed when PATH is modified.
*/

t_ht					*g_binaries;

/*
** main functions
*/

int					eval_command(t_command *command);
int					eval_and_or(t_connection *and_or);
int					eval_pipeline(t_connection *pipeline, int in, int out);
int					eval_simple_command(t_command *command, int in, int out,
					int fd_to_close);
int					eval_command_list(t_command *command_list);
/*
** launch job/process
*/

int					launch_process(t_process *process, int to_close, bool subshell);
int					launch_job(t_job *job);

/*
** exec
*/

char    			**get_argv(t_simple_cmd *command);
char				*get_exec_path(char *command, t_env *env);
int					exec_builtin(char **argv);
int					exec_binary(char **argv, char **env);

/*
** job control
*/

void				put_job_fg(t_job *job, bool cont);
void				put_job_bg(t_job *job, bool cont);
void				wait_for_job(t_job *job, bool subshell);
void				continue_job(t_job *job, bool bg);
void				notif_jobs(void);

/*
** job set status
*/

void				update_status(void);
int					mark_status(pid_t pid, int status);
void				set_process_status(t_process *process, int status);
void				mark_job_as_running (t_job *job);

/*
** job get status
*/

bool				job_is_done(t_job *job);
bool				job_is_stopped(t_job *job);

/*
** job utils
*/

t_job				*get_job_percent_format(char *format);
void				update_curr_ptr(t_job *job);
t_job				*get_job(pid_t pgid);
void				add_job(t_job *job);
void				add_process(t_process *process);
void				remove_job_from_list(pid_t pgid);

/*
** job display
*/

t_dstr				*format_job(t_command *command, t_dstr *buf);
void				print_job(t_job *job, int status);

/*
** job new/del
*/

t_process			*process_new(t_command *cmd, int stdin, int stdout);
t_job				*job_new(t_command *cmd, int stdin, int stdout);
void				process_del(t_process **process);
void				job_del(t_job **job);

/*
** redirections
*/

int					set_redir(t_simple_cmd *command, bool backup);
int					open_heredoc(t_dstr *heredoc);
bool				is_valid_fd(int fd);
void				move_fd(int *fd);
int					dup2_and_backup(int fildes1, int fildes2, bool backup);
int					restore_fds(void);

/*
** error handling
*/

int					redir_error(int code);
int					command_not_found(char *command_name);

/*
** fork utils
*/

void				interrupt_fork(int sig);
void				kill_all_forks(void);

/*

int				exec_pipe(t_node *ast, t_env *env);
int				exec_with_io(t_node *cmd, t_env *env, int stdin_fd,
				int stdout_fd);
int				exec_command_env(char **argv, t_env *env);
int				exec_builtin(t_argv *argv, t_env *env, t_node *cmd,
				bool free_argv);

*/

#endif
