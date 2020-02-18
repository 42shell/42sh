/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_spec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/06 01:18:08 by fratajcz          #+#    #+#             */
/*   Updated: 2019/12/06 01:18:56 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readline.h"

static int	is_word_sep(char c)
{
	return (ft_iswhitespace(c) || ft_isquote(c));
}

int			rl_move_home(void)
{
	while (g_rl_line.i > 0)
		rl_move_left();
	return (0);
}

int			rl_move_end(void)
{
	while (g_rl_line.i < g_rl_line.dstr->len)
		rl_move_right();
	return (0);
}

int			rl_move_prevw(void)
{
	while (g_rl_line.i > 0
	&& is_word_sep(g_rl_line.dstr->str[g_rl_line.i - 1]))
		rl_move_left();
	while (g_rl_line.i > 0
	&& !is_word_sep(g_rl_line.dstr->str[g_rl_line.i - 1]))
		rl_move_left();
	return (0);
}

int			rl_move_nextw(void)
{
	while (g_rl_line.i < g_rl_line.dstr->len
	&& is_word_sep(g_rl_line.dstr->str[g_rl_line.i]))
		rl_move_right();
	while (g_rl_line.i < g_rl_line.dstr->len
	&& !is_word_sep(g_rl_line.dstr->str[g_rl_line.i]))
		rl_move_right();
	return (0);
}
