/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 12:28:09 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/07 16:25:00 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "shell.h"

int		builtin_env(char **argv, t_array *env);
int		builtin_unsetenv(char **argv, t_array *env);
int		builtin_setenv(char **argv, t_array *env);
void	builtin_exit(char **argv, t_array *env);
int		builtin_echo(char **argv, t_array *env);
int		builtin_export(char **argv, t_array *env);
int		builtin_unset(char **argv, t_array *env);
int		builtin_set(char **argv, t_array *env);
int		builtin_cd(char **argv, t_array *env);
int		builtin_fg(char **argv, t_array *env);
int		builtin_bg(char **argv, t_array *env);
int		builtin_jobs(char **argv, t_array *env);

char	*get_pwd();
char	*get_home_dir();
void	append_curpath_to_pwd(char **curpath);
void	remove_dots(char *curpath);
int		remove_dotdots(char *curpath);

bool	is_builtin(char *str);

#endif
