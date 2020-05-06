/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:09:52 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/06 16:39:16 by fratajcz         ###   ########.fr       */
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
# include <sys/prctl.h>
# include <sys/ioctl.h>
# include <signal.h>
# include <fcntl.h>
# include <limits.h>
# include <dirent.h>
# include "libft.h"
# include "readline.h"
# include "ft_printf.h"
# include "env.h"
# include "lexer.h"
# include "parser.h"
# include "execution.h"
# include "job_control.h"
# include "expansion.h"
# include "builtins.h"
# include "autocomplete.h"
# include "utils.h"

# define V_NOATTR	0
# define V_RDONLY	1
# define V_EXPORT	2
# define V_HIDDEN	4
# define V_SPECIAL	8

typedef char *(*t_var_value_func)(void);

/*
** value_func is used to get the values of special variables like $$.
*/

typedef struct			s_var
{
	char				*name;
	char				*value;
	char				*exportstr;
	int					attributes;
	t_var_value_func	value_func;
}						t_var;

/*
** -fix readline problems
** 		-rl_del??
**		-env/termcaps...
**		-write on 2 ? sounds weird to do that in readline,
**		 maybe dup2 in shell.get_input
**		-rigorous testing
** -proper init handling
**		-tty stuff, empty env, job control...
** -input
**		-redo input_interactive() properly, EOF and INT handling...
** -error handling
**		-^C ^D
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

typedef int				(*t_input_func)(const char *, bool);

int						g_last_exit_st;

typedef struct			s_shell
{
	bool				interactive_mode;
	t_input_func		get_input;
	t_job				*jobs;
	t_job				*curr_job;
	t_job				*prev_job;
	pid_t				pgid;
	struct termios		tmodes;
	t_ht				*vars;
}						t_shell;

t_shell					g_shell;

int						init(int argc, char **argv);
void					del(void);

int						input_batch(const char *prompt, bool heredoc);
int						input_interactive(const char *prompt, bool heredoc);

void					init_sig(void);

void				import_env(char **env);
void				add_var(const char *name, const char *value,
					const int attributes);
void				free_var(void *var_ptr);
char				*get_var_value(const char *name);
void				set_var(const char *name, const char *value,
					const int attributes);
void				set_var_attributes(const char *name, const int attributes);
char				*get_var_value(const char *name);
t_array				*export_env(t_ht *map);
void				unset_var(const char *name);
bool				var_exists(const char *name);
void				set_local_variables(t_simple_cmd *cmd);
void				set_temp_env_variables(t_simple_cmd *cmd, t_array *temp_env);

#endif
