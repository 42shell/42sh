/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   param_exp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 16:36:33 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/17 04:57:14 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** Returns NULL if var name is invalid or braces aren't closed
*/

static char	*get_var_name(char *str, bool brace)
{
	int	i;
	int	limit;

	limit = INT_MAX;
	i = 1;
	if (brace)
	{
		if ((limit = get_end_of_braces(str)) == -1)
			return (NULL);
		i = 2;
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
	if (i == 1 || (brace && (i != limit - 1 || !ft_isalnum(str[i]))))
		return (NULL);
	return (ft_strsub(str, brace ? 2 : 1, i - 1));
}

int			param_expand(t_dstr *str, int *i, bool brace)
{
	char *var_value;
	char *var_name;

	var_name = get_var_name(str->str + *i, brace);
	if (var_name == NULL)
	{
		if (brace)
		{
			ft_dprintf(2, "42sh: %s: bad substitution\n", str->str + *i);
			return (1);
		}
		return (0);
	}
	var_value = get_var_value(var_name);
	ft_dstr_remove(str, *i, ft_strlen(var_name) + (brace ? 3 : 1));
	ft_dstr_insert(str, *i, var_value, ft_strlen(var_value));
	*i += ft_strlen(var_value) - 1;
	free(var_name);
	return (0);
}
