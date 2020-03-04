/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:58:15 by fratajcz          #+#    #+#             */
/*   Updated: 2019/12/15 09:58:21 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	set_env_var(char *var, char *value)
{
	if (get_env_var(var) == NULL)
		add_env_var(var, value);
	else
		replace_env_var(var, value);
}

char	*get_env_var(char *var_name)
{
	int				i;
	char			*match;
	char			*var_equals;

	i = 0;
	var_equals = ft_strjoin(var_name, "=");
	while (g_env->env[i] && ((match = ft_strstr(g_env->env[i], var_equals)) == NULL
			|| match != g_env->env[i]))
		i++;
	free(var_equals);
	if (g_env->env[i] == NULL)
		return (NULL);
	return (g_env->env[i] + ft_strlen(var_name) + 1);
}

void	add_env_var(char *var, char *value)
{
	int		i;
	char	**new;

	i = 0;
	if (value == NULL)
		value = "";
	if (g_env->size < g_env->capacity)
	{
		g_env->env[g_env->size - 1] = ft_strjoin_triple(var, "=", value);
		g_env->env[g_env->size] = NULL;
		g_env->size++;
	}
	else
	{
		i = -1;
		new = ft_xmalloc((g_env->capacity * 1.125 + 6) * sizeof(char *));
		g_env->capacity = g_env->capacity * 1.125 + 6;
		while (++i < g_env->size - 1)
			new[i] = g_env->env[i];
		new[i] = ft_strjoin_triple(var, "=", value);
		new[i + 1] = NULL;
		g_env->size++;
		free(g_env->env);
		g_env->env = new;
	}
}

void	replace_env_var(char *var, char *value)
{
	int		i;
	char	*match;

	if (value == NULL)
		value = "";
	i = 0;
	while (g_env->env[i] && ((match = ft_strstr(g_env->env[i], var)) == NULL
					|| match != g_env->env[i]))
		i++;
	if (g_env->env[i] != NULL)
	{
		free(g_env->env[i]);
		g_env->env[i] = ft_strjoin_triple(var, "=", value);
	}
}

void	remove_env_var(char *name)
{
	int		i;
	void	*ptr_to_free;
	char	*match;
	char	*search;

	i = 0;
	search = ft_strjoin(name, "=");
	while (g_env->env[i]
		&& (!(match = ft_strstr(g_env->env[i], search)) || match != g_env->env[i]))
		i++;
	if (g_env->env[i] != NULL)
	{
		ptr_to_free = g_env->env[i];
		while (g_env->env[i])
		{
			g_env->env[i] = g_env->env[i + 1];
			i++;
		}
		g_env->size--;
		free(ptr_to_free);
	}
	free(search);
}
