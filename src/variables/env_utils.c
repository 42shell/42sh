/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:58:15 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/06 16:41:23 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	set_env_var(char *var, char *value, t_array *env)
{
	if (get_env_var(var, env) == NULL)
		add_env_var(var, value, env);
	else
		replace_env_var(var, value, env);
}

char	*get_env_var(char *var_name, t_array *env)
{
	size_t			i;
	char			*match;
	char			*var_equals;

	i = 0;
	var_equals = ft_strjoin(var_name, "=");
	while (i < env->size
			&& ((match = ft_strstr(env->array[i], var_equals)) == NULL
			|| match != env->array[i]))
		i++;
	free(var_equals);
	if (i == env->size)
		return (NULL);
	return (env->array[i] + ft_strlen(var_name) + 1);
}

void	add_env_var(char *var, char *value, t_array *env)
{
	if (value == NULL)
		value = "";
	array_append(env, ft_strjoin_triple(var, "=", value));
}

void	replace_env_var(char *var, char *value, t_array *env)
{
	size_t	i;
	char	*match;
	char	*var_equals;

	if (value == NULL)
		value = "";
	i = 0;
	var_equals = ft_strjoin(var, "=");
	while (i < env->size
			&& ((match = ft_strstr(env->array[i], var_equals)) == NULL
			|| match != env->array[i]))
		i++;
	if (env->array[i] != NULL)
	{
		free(env->array[i]);
		env->array[i] = ft_strjoin(var_equals, value);
	}
	free(var_equals);
}

void	remove_env_var(char *name, t_array *env)
{
	size_t	i;
	void	*ptr_to_free;
	char	*match;
	char	*search;

	i = 0;
	search = ft_strjoin(name, "=");
	while (i < env->size
		&& (!(match = ft_strstr(env->array[i], search))
			|| match != env->array[i]))
		i++;
	if (i < env->size)
	{
		ptr_to_free = env->array[i];
		while (env->array[i])
		{
			env->array[i] = env->array[i + 1];
			i++;
		}
		env->size--;
		free(ptr_to_free);
	}
	free(search);
}
