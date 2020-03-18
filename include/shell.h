/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:09:52 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/24 17:41:36 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

#include <stdio.h>

# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/uio.h>
# include <sys/stat.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <fcntl.h>
# include <limits.h>
# include <dirent.h>
# include "libft.h"
# include "readline.h"
# include "ft_printf.h"
# include "env.h"
# include "lexer.h"
# include "parser.h"
# include "exec.h"
# include "expansion.h"
# include "builtins.h"
# include "utils.h"

/*
** -fix readline problems
** 		-rl_del
**		-empty env seems to not return NULL
**		-maybe find better way to handle init and del
**		-write on 2 ? sounds weird to do that in readline, maybe dup2 in shell.get_input 
**		-rigorous testing
** -batch_mode:
** 		-heredocs: no delim before EOF currently quit without error msg
** -empty env -> set default values ?
** -flush cache table when PATH is modified.
** -check leaks everywhere
** -last exit status, close fds, error handling... assert everything is perfect
** -modify env builtin to use g_env
** -use get_next_line in batch mode to be able to remove \\n,
** 	for the moment it is not correct because of BUFF_SIZE
**  we may read until \ and not be aware of following \n
** -empty lines '\n' returns parse error
*/

# define BUFF_SIZE	2
# define INPUT_INT	3
# define INPUT_EOF	4

# define PS1		"$> "
# define PS2		"> "
# define PSQ		"q> "
# define PSD		"d> "
# define PSA		"a> "
# define PSO		"o> "
# define PSP		"p> "
# define PSH		"h> "

typedef int			(*t_input_func)(const char *);

int					g_last_exit_st;

typedef struct		s_shell
{
	bool			interactive_mode;
	t_input_func	get_input;
	t_job			*jobs;
	pid_t			pgid;
}					t_shell;

t_shell				g_shell;

int					init(int argc, char **argv);
void				del(void);

int					input_batch(const char *prompt);
int					input_interactive(const char *prompt);

void				init_sig(void);
void				sig_handle(int sig);

#endif
