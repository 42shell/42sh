/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arith_exp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/06 15:58:15 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/11 02:42:10 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

extern bool g_arith_toomuch;

#define ERR 1

extern int g_arith_status;

int			arith_expand(t_dstr *str, int *i)
{
	int		end;
	char	*buf;

	end = get_end_of_braces(str->str + *i);
	str->str[end + *i] = '\0';
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
