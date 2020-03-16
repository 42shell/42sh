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

typedef struct	s_fd_backup
{
	int			backup;
	int			orig_number;
}				t_fd_backup;

/*
** associative table of command names and binaries paths.
** -an entry is added when a command is executed if the command is not the table.
** -the table is flushed when PATH is modified.
*/

t_ht			*g_binaries;

int				execute_ast(t_node *node);
int				exec_command(t_command *command);
char			*get_exec_path(char *command);

/*
** redirections
*/

int				set_redir(t_command *command, bool backup);
int				open_heredoc(t_dstr *heredoc);
bool			is_valid_fd(int fd);
void			move_fd(int *fd);
int				dup2_and_backup(int fildes1, int fildes2, bool backup);
int				restore_fds(void);

/*
** pipes
*/

int				set_pipe_redir(int input_fd, int fildes[2]);

/*
** error handling
*/

int				redir_error(int code);
int				command_not_found(char *command_name);

/*
** fork utils
*/

void			interrupt_fork(int sig);
void			kill_all_forks(void);

/*

int				exec_pipe(t_node *ast, t_env *env);
int				exec_with_io(t_node *cmd, t_env *env, int stdin_fd,
				int stdout_fd);
int				exec_command_env(char **argv, t_env *env);
int				exec_builtin(t_argv *argv, t_env *env, t_node *cmd,
				bool free_argv);

*/

#endif
