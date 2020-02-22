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

bool				g_shell_interactive;

typedef struct		s_sh
{
	struct s_lexer	lexer;
	struct s_env	env;
}					t_sh;

int					init(t_sh *shell, int argc, char **argv);
void				del();

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

#endif
