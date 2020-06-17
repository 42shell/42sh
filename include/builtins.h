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
** utils
*/

bool		is_builtin(char *str);
int			exec_command_env(char **argv, t_array *env);

#endif
