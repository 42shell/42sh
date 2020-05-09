/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_basic.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/06 01:18:08 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 17:20:51 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readline.h"

static int	get_prevline_len(void)
{
	int		prevline_len;
	int		line_i;

	prevline_len = 0;
	line_i = (int)g_rl_line.i - 1;
	if (line_i > -1 && g_rl_line.dstr->str[line_i] == '\n')
	{
		line_i--;
		prevline_len++;
	}
	while (line_i > -1 && g_rl_line.dstr->str[line_i--] != '\n')
		prevline_len++;
	if (line_i == -1)
		prevline_len += (int)g_rl_prompt_len;
	if (prevline_len > (int)g_rl_sizex && !(prevline_len %= (int)g_rl_sizex))
		prevline_len = (int)g_rl_sizex;
	return (prevline_len);
}

int			rl_move_left(void)
{
	int		prevline_len;
	int		offset;

	if (g_rl_line.i > 0)
	{
		if (g_rl_line.dstr->str[g_rl_line.i - 1] == '\n')
		{
			prevline_len = get_prevline_len() - 1;
			offset = g_rl_sizex - prevline_len;
			while (offset--)
				movcleft();
		}
		else
			movcleft();
		g_rl_line.i--;
	}
	return (0);
}

int			rl_move_right(void)
{
	if (g_rl_line.i < g_rl_line.dstr->len)
	{
		if (g_rl_line.dstr->str[g_rl_line.i] == '\n')
			movcnl();
		else
			movcright();
		g_rl_line.i++;
	}
	return (0);
}
