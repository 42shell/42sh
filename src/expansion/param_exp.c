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

static void	replace_name_by_value(t_token *token, char *var_name, int *i,
		bool brace)
{
	char	*var_value;
	size_t	to_remove_len;
	size_t	value_len;

	var_value = get_var_value(var_name);
	to_remove_len = ft_strlen(var_name) + (brace ? 3 : 1);
	value_len = ft_strlen(var_value);
	ft_dstr_remove(token->value, *i, to_remove_len);
	ft_dstr_insert(token->value, *i, var_value, value_len);
	if (token->exp_info)
	{
		ft_dstr_remove(token->exp_info, *i, to_remove_len);
		ft_dstr_insert(token->exp_info, *i, var_value, value_len);
		ft_memset(token->exp_info->str + *i, '1', value_len);
	}
	*i += value_len - 1;
	free(var_name);
}

int			param_expand(t_token *token, int *i, bool brace)
{
	char	*var_name;

	var_name = get_var_name(token->value->str + *i, brace);
	if (var_name == NULL)
	{
		if (brace)
		{
			ft_dprintf(2, "42sh: %s: bad substitution\n",
					token->value->str + *i);
			return (1);
		}
		return (0);
	}
	replace_name_by_value(token, var_name, i, brace);
	return (0);
}
