/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 12:28:09 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/14 16:16:00 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "shell.h"

int		builtin_env(char **argv, t_env *env);
int		builtin_unsetenv(char **argv);
int		builtin_setenv(char **argv);
void	builtin_exit(char **argv);
int		builtin_echo(char **argv);
int		builtin_cd(char **argv);
int		builtin_fg(char **argv);

char	*get_pwd();
char	*get_home_dir();
void	append_curpath_to_pwd(char **curpath);
void	remove_dots(char *curpath);
int		remove_dotdots(char *curpath);

bool	is_builtin(char *str);

#endif
