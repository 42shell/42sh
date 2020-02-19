/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:09:52 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/19 14:47:46 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

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
# include "ft_printf.h"
# include "env.h"
# include "terminal.h"
# include "input.h"
# include "lexer.h"
# include "parser.h"
# include "exec.h"
# include "expansion.h"
# include "builtins.h"

t_ht	*g_sh_vars;

typedef struct		s_sh
{
	struct s_term	term;
	struct s_input	input;
	struct s_lexer	lexer;
	struct s_env	env;
}					t_sh;

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

int					init(t_sh *shell, int argc, char **argv);
void				del(t_sh *shell);

void				init_sig(t_sh *shell);
void				sig_handle(int sig);
void				sig_action(t_sh *shell, int sig);

char				*ft_strjoin_triple(char *s1, char *s2, char *s3);
void				free_arr(char **arr);

t_env				env_dup(char **env);
char				*get_env_var(char *var_name, t_env *env);
void				add_env_var(char *var, char *value, t_env *env);
void				replace_env_var(char *var, char *value, t_env *env);
void				remove_env_var(char *name, t_env *env);
void				set_env_var(char *var, char *value, t_env *env);

char				*get_executable_path(char *command, t_env *env);
char				*append_filename(char *path, char *filename);
char				**split_path(char const *path);

char				get_opt(int argc, char *argv[]);
bool				is_builtin(char *str);

bool				is_valid_var_name(char *str);

char				*ft_mktemp(char *template);

void				import_env(t_ht *map, char **env);
void				add_var(t_ht *map, char *name, char *value, int attributes);
void				free_var(void *var_ptr);

#endif
