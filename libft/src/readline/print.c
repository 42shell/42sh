/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:08:08 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/16 15:39:06 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readline.h"

void		rl_carriage_return(bool echo_missing_nl)
{
	size_t	i;

	i = 0;
	if (echo_missing_nl)
	{
		tputs(tgetstr("mr", NULL), 1, ft_putc);
		tputs("%", 1, ft_putc);
		tputs(tgetstr("me", NULL), 1, ft_putc);
		g_rl_posx++;
	}
	while (i++ < g_rl_sizex - 2)
	{
		if (g_rl_posy == g_rl_sizey - 1 && g_rl_posx == g_rl_sizex - 1)
			movcnl();
		else
			movcright();
	}
	tputs(g_rl_caps[C_CR], 1, ft_putc);
	g_rl_posx = 0;
}

void		rl_print_prompt(const char *prompt)
{
	if (g_rl_prompt_cr)
		rl_carriage_return(true);
	tputstr(prompt);
	g_rl_prompt = prompt;
	g_rl_prompt_len = ft_strlen(prompt);
}

void		rl_print_line(bool restore_i)
{
	tputs(g_rl_caps[C_CD], 1, ft_putc);
	g_rl_line.i = tputstr(g_rl_line.dstr->str);
	if (restore_i)
	{
		while (g_rl_line.i > 0)
			rl_move_left();
	}
}

void		rl_print_line_from_i(bool restore_i)
{
	size_t	old_i;

	old_i = g_rl_line.i;
	tputs(g_rl_caps[C_CD], 1, ft_putc);
	g_rl_line.i += tputstr(&g_rl_line.dstr->str[g_rl_line.i]);
	if (restore_i)
	{
		while (g_rl_line.i > old_i)
			rl_move_left();
	}
}
