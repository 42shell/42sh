/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_features.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:08:08 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/16 15:39:06 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readline.h"

int		rl_enter(void)
{
	return (10);
}

int		rl_delete(void)
{
	if (g_rl_line.i == g_rl_line.dstr->len)
		return (0);
	ft_dstr_remove(g_rl_line.dstr, g_rl_line.i, 1);
	rl_print_line_from_i(true);
	return (0);
}

int		rl_backspace(void)
{
	if (g_rl_line.i == 0)
		return (0);
	rl_move_left();
	ft_dstr_remove(g_rl_line.dstr, g_rl_line.i, 1);
	rl_print_line_from_i(true);
	return (0);
}

int		rl_insert_char(void)
{
	if (g_rl_line_limit
	&& g_rl_line.dstr->len == g_rl_line_size_max)
		return (0);
	else if (ft_dstr_insert(g_rl_line.dstr, g_rl_line.i,
	(char *)g_rl_key.bytes, 1) == -1)
	{
		g_rl_error = RL_ALLOC_ERROR;
		return (-1);
	}
	rl_print_line_from_i(true);
	rl_move_right();
	return (0);
}
