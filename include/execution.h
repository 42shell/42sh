/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 15:00:39 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/05 17:19:13 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "shell.h"
#include <errno.h>

# define ERROR_REDIR_OPEN		-1
# define ERROR_REDIR_BAD_FD		-2

# define EXEC_ASYNC				1			
# define EXEC_AND_OR			2
# define EXEC_PIPELINE			4
# define EXEC_ALREADY_FORKED	8

# define JOB_DONE				1		
# define JOB_STOPPED			2		

typedef struct		s_fd_backup
{
	int				backup;
	int				orig_number;
}					t_fd_backup;

/*
** associative table of command names and binaries paths.
** -an entry is added when a command is executed if the command is not the table.
** -the table is (should be) flushed when PATH is modified.
*/

t_ht				*g_binaries;

bool				g_job_control_enabled;
bool				g_already_forked;

/*
** eval tree
*/

int					eval_command_list(t_command *command_list);
int					eval_command(t_command *command);
int					eval_and_or(t_command *command);
int					eval_pipeline(t_command *command, int in, int out);
int					eval_simple_command(t_command *commande);

/*
** execution
*/

int					exec_simple_command(t_simple_cmd *simple);
int					exec_builtin(char **argv, t_env *temp_env);
int					exec_binary(char **argv, t_env *temp_env);
char    			**get_argv(t_simple_cmd *command);
char				*get_exec_path(char *command, t_env *env);

/*
** redirections
*/

int					set_redir(t_simple_cmd *command, bool backup);
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
