/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_dup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:08:08 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/29 14:18:16 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_env		*env_dup(char **environ)
{
	t_env	*env;
	int		i;

	i = 0;
	while (environ[i])
		i++;
	env = ft_xmalloc(sizeof(t_env));
	env->env = ft_xmalloc(((i + 1) * 1.125 + 6) * sizeof(char *));
	env->capacity = (i + 1) * 1.125 + 6;
	env->size = i + 1;
	i = 0;
	while (environ[i])
	{
		env->env[i] = ft_strdup(environ[i]);
		i++;
	}
	env->env[i] = NULL;
	return (env);
}
