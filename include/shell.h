/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:09:52 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/17 04:53:40 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

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

/*
** vars.h
*/

# define V_NOATTR	0
# define V_RDONLY	1
# define V_EXPORT	2
# define V_HIDDEN	4
# define V_SPECIAL	8

typedef char	*(*t_var_value_func)(void);
typedef int		(*t_builtin_func)(char **argv, t_array *temp_env);

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
** TODO:
** -commands reading from stdin in batch mode
** -handle job control on exit
** -bangs/shell script
**
** problems:
** -file_is_binary "ls"
** -watch deceitful lexer
** -store aborted multi-lines in history ? bash does
** -prompt when executing tests ?
**
** old problems:
** -unsetenv boucle inf when unseting non existing var ?
** -env dup invalid read ?
** -autcomplete vim /t doesnt complete files ?
** -others
** 		-check comments work properly
** -leaks/invalid reads/still reachable
*/

# define INPUT_INT		3
# define INPUT_EOF		4

# define PS1			"$> "
# define PS2			"\\... "
# define PSQ			"'... "
# define PSD			"\"... "
# define PSA			"&&... "
# define PSO			"||... "
# define PSP			"|... "
# define PSH			"<<... "
# define PSB			"(... "
# define PSDP			"((... "
# define PSC			"{... "

typedef int				(*t_input_func)(const char *, bool);

extern int				g_last_exit_st;

typedef struct			s_shell
{
	bool				interactive_mode;
	t_input_func		get_input;
	t_job				*jobs;
	pid_t				pgid;
	struct termios		tmodes;
	t_ht				*vars;
}						t_shell;

extern t_shell			g_shell;
extern t_ht				*g_builtins;

int						init(int argc, char **argv);
void					del(void);

int						input_batch(const char *prompt, bool heredoc);
int						input_interactive(const char *prompt, bool heredoc);
bool					file_is_binary(char *filename);
void					normalize_lexer_line(void);

void					init_sig(void);

void					import_env(char **env);
t_var					*make_new_var(const char *name, const char *value,
						const int attributes, t_var_value_func value_func);
void					add_var(const char *name, const char *value,
						const int attributes);
void					free_var(void *var_ptr);
char					*get_var_value(const char *name);
void					set_var(const char *name, const char *value,
						const int attributes);
void					set_var_attributes(const char *name,
						const int attributes);
char					*get_var_value(const char *name);
t_array					*export_env(t_ht *map);
void					unset_var(const char *name);
bool					var_exists(const char *name);
void					set_local_variables(t_simple_cmd *cmd);
void					set_temp_env_variables(t_simple_cmd *cmd,
						t_array *temp_env);
char					*get_last_exit_status(void);
char					*get_shell_pid(void);
char					*get_last_bg_job_pid(void);

# define BRACK_CAN_OPEN		0x1
# define BRACK_CAN_CLOSE	0x2

enum e_quote_st			get_bracket_status(t_array *stack);
void					set_bracket_status(const char *str, int i,
						t_array *stack, bool can_open);
void					add_bracket_to_stack(t_array *stack,
						enum e_quote_st bracket);
void					pop_bracket_from_stack(t_array *stack);

#endif
