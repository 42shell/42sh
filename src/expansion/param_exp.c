/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   param_exp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 16:36:33 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/29 22:46:52 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** quotes inside braces are handled separately from the global quote status
** i.e echo "${HOME"}"}" is one substitution
*/

static bool g_brace;

static int	get_end_of_braces(const char *str)
{
	int		i;
	char	quote_status;

	i = -1;
	quote_status = NONE;
	while (str[++i])
	{
		if (quote_start(str, i, &quote_status))
			continue ;
		if (quote_status == NONE && str[i] == '}')
			return (i);
		quote_stop(str, i, &quote_status);
	}
	return (-1);
}

static char	*get_var_name(char *str)
{
	int	i;
	int	limit;

	limit = INT_MAX;
	i = 0;
	if (str[0] == '{')
	{
		g_brace = true;
		if ((limit = get_end_of_braces(str)) == -1)
			return (NULL);
		i++;
	}
	if (str[i] == '$' || str[i] == '!' || str[i] == '?')
	{
		if (!g_brace || str[i + 1] == '}')
			return (ft_strsub(str, i, 1));
	}
	if (ft_isdigit(str[i]))
		return (NULL);
	while ((ft_isalnum(str[i]) || str[i] == '_') && i < limit - 1)
		i++;
	if (i == 0 || (g_brace && i != limit - 1))
		return (NULL);
	return (ft_strsub(str, g_brace ? 1 : 0, i));
}

static bool	should_expand(char *str, int i, char quote_status, bool heredoc)
{
	if (str[i] != '$')
		return (false);
	if ((quote_status == SQUOTE && !heredoc) || quote_status == BSLASH)
		return (false);
	return (true);
}

static void	expand_var(t_dstr *str, int *i, const char *var_name)
{
	char *var_value;

	var_value = get_var_value(var_name);
	ft_dstr_remove(str, *i, ft_strlen(var_name) + (g_brace ? 3 : 1));
	ft_dstr_insert(str, *i, var_value, ft_strlen(var_value));
	*i += ft_strlen(var_value) - 1;
}

char	*g_expand_error_token = NULL;

int			param_expand(t_dstr *str, int start, bool heredoc)
{
	char	*var_name;
	int		i;
	char	quote_status;

	ft_memdel((void**)&g_expand_error_token);
	g_expand_error_token = ft_strdup(str->str);
	i = start - 1;
	quote_status = NONE;
	var_name = NULL;
	while (str->str[++i])
	{
		if (quote_start(str->str, i, &quote_status))
			continue ;
		g_brace = false;
		if (should_expand(str->str, i, quote_status, heredoc)
				&& (var_name = get_var_name(str->str + i + 1)))
		{
			expand_var(str, &i, var_name);
			free(var_name);
		}
		if (var_name == NULL && g_brace)
			return (1);
		if (i >= 0)
			quote_stop(str->str, i, &quote_status);
	}
	return (0);
}
