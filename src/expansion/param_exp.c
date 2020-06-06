/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   param_exp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 16:36:33 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/05 23:31:55 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** quotes inside braces are handled separately from the global quote status
** i.e echo "${HOME"}"}" is one substitution
*/

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

/*
** Returns NULL if var name is invalid or braces aren't closed
*/

static char	*get_var_name(char *str, bool brace)
{
	int	i;
	int	limit;

	limit = INT_MAX;
	i = 0;
	if (str[0] == '{')
	{
		if ((limit = get_end_of_braces(str)) == -1)
			return (NULL);
		i++;
	}
	if (str[i] == '$' || str[i] == '!' || str[i] == '?')
	{
		if (!brace || str[i + 1] == '}')
			return (ft_strsub(str, i, 1));
	}
	if (ft_isdigit(str[i]))
		return (NULL);
	while ((ft_isalnum(str[i]) || str[i] == '_') && i < limit - 1)
		i++;
	if (i == 0 || (brace && i != limit - 1))
		return (NULL);
	return (ft_strsub(str, brace ? 1 : 0, i));
}

int			param_expand(t_dstr *str, int *i, bool brace)
{
	char *var_value;
	char *var_name;

	var_name = get_var_name(str->str + *i + 1, brace);
	if (var_name == NULL)
		return (brace ? 1 : 0);
	var_value = get_var_value(var_name);
	ft_dstr_remove(str, *i, ft_strlen(var_name) + (brace ? 3 : 1));
	ft_dstr_insert(str, *i, var_value, ft_strlen(var_value));
	*i += ft_strlen(var_value) - 1;
	free(var_name);
	return (0);
}
