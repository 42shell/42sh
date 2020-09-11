/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 12:28:09 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/28 20:10:11 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "shell.h"

int			builtin_env(char **argv, t_array *env);
int			builtin_unsetenv(char **argv, t_array *env);
int			builtin_setenv(char **argv, t_array *env);
void		builtin_exit(char **argv, t_array *env);
int			builtin_echo(char **argv, t_array *env);
int			builtin_export(char **argv, t_array *env);
int			builtin_unset(char **argv, t_array *env);
int			builtin_set(char **argv, t_array *env);
int			builtin_cd(char **argv, t_array *env);
int			builtin_fg(char **argv, t_array *env);
int			builtin_bg(char **argv, t_array *env);
int			builtin_jobs(char **argv, t_array *env);
int			builtin_test(char **argv, t_array *env);

/*
** jobs/fgbg utils
*/

t_job		*get_job_by_str(char *str);

/*
** cd utils
*/

char		*get_pwd();
char		*get_home_dir(t_array *env);
void		append_curpath_to_pwd(char **curpath);
void		remove_dots(char *curpath);
int			remove_dotdots(char *curpath);

/*
** test operations
*/

int			test_b(char **argv);
int			test_c(char **argv);
int			test_d(char **argv);
int			test_e(char **argv);
int			test_f(char **argv);
int			test_g(char **argv);
int			test_l(char **argv);
int			test_p(char **argv);
int			test_r(char **argv);
int			test_sock(char **argv);
int			test_size(char **argv);
int			test_t(char **argv);
int			test_u(char **argv);
int			test_w(char **argv);
int			test_x(char **argv);
int			test_z(char **argv);
int			test_equal(char **argv);
int			test_unequal(char **argv);
int			test_eq(char **argv);
int			test_ne(char **argv);
int			test_gt(char **argv);
int			test_ge(char **argv);
int			test_lt(char **argv);
int			test_le(char **argv);

/*
** utils
*/

bool		is_builtin(char *str);
int			exec_command_env(char **argv, t_array *env);
bool		is_number(char *str);

#endif
