/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_dup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:08:08 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/06 16:31:13 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_array	*env_dup(char **environ)
{
	t_array *env;
	int		i;

	i = 0;
	while (environ[i])
		i++;
	env = array_new(16);
	i = 0;
	while (environ[i])
	{
		array_append(env, ft_strdup(environ[i]));
		i++;
	}
	return (env);
}
