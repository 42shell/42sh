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
**		-env/termcaps...
**		-better way to handle init and del
**		-write on 2 ? sounds weird to do that in readline,
**		 maybe dup2 in shell.get_input 
**		-rigorous testing
** -proper init handling
**		-tty stuff, empty env, job control...
** -input
**		-redo input_interactive() properly, EOF et INT handling...
** 		-do input_batch, should be easier cause lexer bugs have been fixed,
**		 use gnl equivalent
** -error handling
**		-^C ^D
**		-improve parse error handling ("near token `newline`")
**		-last exit status, close fds... assert everything is perfect
** -others
** 		-flush cache table when PATH is modified.
** 		-check comments work properly
** 		-in case of "ls | \n cat", newline is not removed from line before stored in history
**		 don t know if it is handled in expand()
** -leaks
*/

# define INPUT_INT		3
# define INPUT_EOF		4

# define PS1			"$> "
# define PS2			"> "
# define PSQ			"q> "
# define PSD			"d> "
# define PSA			"a> "
# define PSO			"o> "
# define PSP			"p> "
# define PSH			"h> "

typedef int				(*t_input_func)(const char *);

int						g_last_exit_st;

typedef struct			s_shell
{
	bool				interactive_mode;
	t_input_func		get_input;
	t_complete_command	*commands;
	pid_t				pgid;
}						t_shell;

t_shell					g_shell;

int						init(int argc, char **argv);
void					del(void);

int						input_batch(const char *prompt);
int						input_interactive(const char *prompt);

void					init_sig(void);
void					sig_handle(int sig);

#endif
