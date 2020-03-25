/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 15:00:39 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/07 00:39:57 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "shell.h"

# define ERROR_REDIR_OPEN	-1
# define ERROR_REDIR_BAD_FD	-2

typedef struct			s_fd_backup
{
	int					backup;
	int					orig_number;
}						t_fd_backup;

typedef struct			s_process
{
	struct s_process	*next;
	t_node				*ast;
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
	t_node				*ast;
	t_process			*processes;
	pid_t				pgid;
	int					stdin;
	int					stdout;
	bool				notified;
	bool				bg;
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


int					eval_ast(t_node *ast);
int					eval_and_or(t_node *ast);
int					eval_pipeline(t_node *ast, int in, int out);
int					eval_command(t_node *ast);
int					eval_simple_command(t_node *ast);

t_process			*process_new(t_node *ast, int stdin, int stdout);
t_job				*job_new(t_node *ast, int stdin, int stdout);
int					launch_process(t_process *process, int to_close);
int					launch_job(t_job *job);
int					exec_builtin(char **argv, char **env);
int					exec_binary(char **argv, char **env);

bool				job_is_done(t_job *job);
void				add_job(t_job *job);
void				add_process(t_process *process);
void				wait_for_job(t_job *job);

char				**get_argv(t_command *command);
char				*get_exec_path(char *command);

/*
** redirections
*/

int					set_redir(t_process *process, bool backup);
int					open_heredoc(t_dstr *heredoc);
bool				is_valid_fd(int fd);
void				move_fd(int *fd);
int					dup2_and_backup(int fildes1, int fildes2, bool backup);
int					restore_fds(void);

/*
** pipes
*/

int					set_pipe_redir(int input_fd, int fildes[2]);

/*
** error handling
*/

int					redir_error(int code);
int					command_not_found(char *command_name);

/*
** job control
*/

t_job				*get_job(pid_t pgid);
bool				job_is_stopped(t_job *job);
//bool				pipeline_is_done(t_pipeline *pipeline);
void				put_job_fg(t_job *job, bool cont);

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
