/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arith_exp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/06 15:58:15 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/11 02:42:30 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

extern bool g_arith_toomuch;

/*
** Only dquotes and backslashes in front of dquotes are removed. why? idk
*/

static void	remove_dquotes(t_dstr *str, int start)
{
	int i;

	i = start;
	while (str->str[i])
	{
		if (str->str[i] == '\\' && str->str[i + 1] == '\"')
			ft_dstr_remove(str, i++, 1);
		else if (str->str[i] == '\"')
			ft_dstr_remove(str, i, 1);
		else
			i++;
	}
}

#define ERR 1

extern int g_arith_status;

int			arith_expand(t_dstr *str, int *i)
{
	int		end;
	char	*buf;

	end = get_end_of_braces(str->str + *i);
	str->str[end + *i] = '\0';
	remove_dquotes(str, *i + 3);
	if (dollar_expand(str, *i + 3, false) == 1)
		return (1);
	buf = ft_itoa_base(eval_expr(str->str + *i + 3), 10);
	if (g_arith_toomuch || g_arith_status == ERR)
	{
		free(buf);
		return (1);
	}
	ft_dstr_remove(str, *i, ft_strlen(str->str + *i) + 2);
	ft_dstr_insert(str, *i, buf, ft_strlen(buf));
	*i += ft_strlen(buf) - 1;
	free(buf);
	return (0);
}
