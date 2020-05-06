/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   param_exp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 16:36:33 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/05 15:53:49 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static char	*get_var_name(char *str)
{
	int	i;

	if (ft_isdigit(str[0]))
		return (NULL);
	i = 0;
	while (ft_isalnum(str[i]) || str[i] == '_')
		i++;
	if (i == 0)
		return (NULL);
	return (ft_strsub(str, 0, i));
}

static bool	should_expand(char *str, int i, char quote_status, bool heredoc)
{
	if (str[i] != '$')
		return (false);
	if ((quote_status == SQUOTE && !heredoc) || quote_status == BSLASH)
		return (false);
	return (true);
}

int			param_expand(t_dstr *str, int start, bool heredoc)
{
	char	*var_name;
	char	*var_value;
	int		i;
	char	quote_status;

	i = start - 1;
	quote_status = NONE;
	while (str->str[++i])
	{
		if (quote_start(str->str, i, &quote_status))
			continue ;
		if (should_expand(str->str, i, quote_status, heredoc)
				&& (var_name = get_var_name(str->str + i + 1)))
		{
			var_value = get_var_value(var_name);
			ft_dstr_remove(str, i, ft_strlen(var_name) + 1);
			ft_dstr_insert(str, i, var_value, ft_strlen(var_value));
			i += ft_strlen(var_value) - 1;
			free(var_name);
		}
		if (i >= 0)
			quote_stop(str->str, i, &quote_status);
	}
	return (0);
}
