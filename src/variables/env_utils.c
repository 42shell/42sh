/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:58:15 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/06 15:26:04 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	set_env_var(char *var, char *value, t_env *env)
{
	if (get_env_var(var, env) == NULL)
		add_env_var(var, value, env);
	else
		replace_env_var(var, value, env);
}

char	*get_env_var(char *var_name, t_env *env)
{
	int				i;
	char			*match;
	char			*var_equals;

	i = 0;
	var_equals = ft_strjoin(var_name, "=");
	while (env->env[i] && ((match = ft_strstr(env->env[i], var_equals)) == NULL
			|| match != env->env[i]))
		i++;
	free(var_equals);
	if (env->env[i] == NULL)
		return (NULL);
	return (env->env[i] + ft_strlen(var_name) + 1);
}

void	add_env_var(char *var, char *value, t_env *env)
{
	int		i;
	char	**new;

	if (value == NULL)
		value = "";
	if (env->size < env->capacity)
	{
		env->env[env->size - 1] = ft_strjoin_triple(var, "=", value);
		env->env[env->size] = NULL;
		env->size++;
	}
	else
	{
		i = -1;
		new = ft_xmalloc((env->capacity * 1.125 + 6) * sizeof(char *));
		env->capacity = env->capacity * 1.125 + 6;
		while (++i < env->size - 1)
			new[i] = env->env[i];
		new[i] = ft_strjoin_triple(var, "=", value);
		new[i + 1] = NULL;
		env->size++;
		free(env->env);
		env->env = new;
	}
}

void	replace_env_var(char *var, char *value, t_env *env)
{
	int		i;
	char	*match;

	if (value == NULL)
		value = "";
	i = 0;
	while (env->env[i] && ((match = ft_strstr(env->env[i], var)) == NULL
					|| match != env->env[i]))
		i++;
	if (env->env[i] != NULL)
	{
		free(env->env[i]);
		env->env[i] = ft_strjoin_triple(var, "=", value);
	}
}

void	remove_env_var(char *name, t_env *env)
{
	int		i;
	void	*ptr_to_free;
	char	*match;
	char	*search;

	i = 0;
	search = ft_strjoin(name, "=");
	while (env->env[i]
		&& (!(match = ft_strstr(env->env[i], search)) || match != env->env[i]))
		i++;
	if (env->env[i] != NULL)
	{
		ptr_to_free = env->env[i];
		while (env->env[i])
		{
			env->env[i] = env->env[i + 1];
			i++;
		}
		env->size--;
		free(ptr_to_free);
	}
	free(search);
}
