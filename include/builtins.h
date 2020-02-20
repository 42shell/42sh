/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 12:28:09 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/20 19:55:35 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "shell.h"

int		builtin_env(char **argv, t_env *env);
int		builtin_unsetenv(char **argv, t_env *env);
int		builtin_setenv(char **argv, t_env *env);
void	builtin_exit(char **argv);
int		builtin_echo(char **argv);
int		builtin_export(char **argv);
int		builtin_unset(char **argv);

int		builtin_cd(char **argv, t_env *env);
char	*get_pwd(t_env *env);
char	*get_home_dir(t_env *env);
void	append_curpath_to_pwd(char **curpath, t_env *env);
void	remove_dots(char *curpath);
int		remove_dotdots(char *curpath);

#endif
