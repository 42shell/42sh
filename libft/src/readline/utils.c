/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/06 01:18:08 by fratajcz          #+#    #+#             */
/*   Updated: 2019/12/06 01:18:56 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readline.h"

static void	redraw_line(void)
{
	size_t old_pos;

	old_pos = g_rl_line.i;
	movcnl();
	rl_print_prompt(g_rl_prompt);
	rl_print_line(false);
	while (g_rl_line.i != old_pos)
		rl_move_left();
}

bool		y_n_prompt(void)
{
	u_int64_t c;

	c = 0;
	while (read(0, &c, 8))
	{
		if (c == 'n')
		{
			redraw_line();
			return (false);
		}
		if (c == 'y')
			return (true);
		if (c == 3)
		{
			rl_interrupt();
			redraw_line();
			return (false);
		}
		c = 0;
	}
	return (false);
}

int			is_word_sep(char c)
{
	return (ft_iswhitespace(c) || ft_isquote(c));
}
