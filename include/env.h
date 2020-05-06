/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 12:28:09 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/06 16:31:53 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "shell.h"

t_array 			*env_dup(char **env);
char				*get_env_var(char *var_name, t_array *env);
void				add_env_var(char *var, char *value, t_array *env);
void				replace_env_var(char *var, char *value, t_array *env);
void				remove_env_var(char *name, t_array *env);
void				set_env_var(char *var, char *value, t_array *env);

#endif
