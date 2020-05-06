/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 12:28:09 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/06 15:26:09 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "shell.h"

typedef struct		s_env
{
	char			**env;
	int				size;
	int				capacity;
}					t_env;

t_env				*env_dup(char **env);
char				*get_env_var(char *var_name, t_env *env);
void				add_env_var(char *var, char *value, t_env *env);
void				replace_env_var(char *var, char *value, t_env *env);
void				remove_env_var(char *name, t_env *env);
void				set_env_var(char *var, char *value, t_env *env);

#endif
