/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 15:00:39 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/28 19:56:52 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "shell.h"
# include <errno.h>

# define ERROR_REDIR_OPEN		-1
# define ERROR_REDIR_BAD_FD		-2
# define ERROR_REDIR_EXPAND		-3

typedef struct		s_fd_backup
{
	int				backup;
	int				orig_number;
}					t_fd_backup;

/*
** associative table of command names and binaries paths.
** -an entry is added when a command is executed if the command is not the table
** -the table is (should be) flushed when PATH is modified.
*/

extern bool			g_job_control_enabled;
extern bool			g_already_forked;

/*
** to know if we are in backgroud or not during
** execution.
*/

bool				g_bg;

/*
** eval tree
*/

int					eval_complete_command(t_command *command);
int					eval_and_or(t_command *command);
int					eval_pipeline(t_command *command, int in, int out);
int					eval_command(t_command *command);
int					eval_group_command(t_command *command);
int					eval_if_clause(t_command *command);
int					eval_simple_command(t_command *command);

/*
** execution
*/

int					exec_group_command(t_group_cmd *group);
int					exec_simple_command(t_simple_cmd *simple);
int					exec_builtin(char **argv, t_array *temp_env);
int					exec_binary(char **argv, t_array *temp_env);
char				**get_argv(t_simple_cmd *command);
char				*get_exec_path(char *command, t_array *env);

/*
** redirections
*/

int					set_redir(t_redir *redirs, bool backup);
int					open_heredoc(t_dstr *heredoc);
int					dup2_and_backup(int fildes1, int fildes2, bool backup);
bool				is_valid_fd(int fd);
int					restore_fds(void);
void				move_fd(int *fd);

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

#endif
