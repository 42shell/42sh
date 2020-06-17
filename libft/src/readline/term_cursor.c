/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term_cursor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/06 14:59:55 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 18:46:57 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readline.h"

void	movcright(void)
{
	if (g_rl_posx == g_rl_sizex - 1)
	{
		tputs(tgoto(g_rl_caps[C_CH], 0, 0), 1, ft_putc);
		if (g_rl_posy < g_rl_sizey - 1)
		{
			tputs(g_rl_caps[C_SF], 1, ft_putc);
			g_rl_posy++;
		}
		g_rl_posx = 0;
	}
	else
	{
		tputs(g_rl_caps[C_ND], 1, ft_putc);
		g_rl_posx++;
	}
}

void	movcleft(void)
{
	if (g_rl_posx == 0)
	{
		tputs(tgoto(g_rl_caps[C_CH], 0, g_rl_sizex - 1), 20, ft_putc);
		if (g_rl_posy > 0)
		{
			tputs(g_rl_caps[C_SR], 1, ft_putc);
			g_rl_posy--;
		}
		g_rl_posx = g_rl_sizex - 1;
	}
	else
	{
		tputs(g_rl_caps[C_LE], 1, ft_putc);
		g_rl_posx--;
	}
}

void	movcnl(void)
{
	tputs(g_rl_caps[C_SF], 20, ft_putc);
	tputs(g_rl_caps[C_CR], 20, ft_putc);
	if (g_rl_posy < g_rl_sizey - 1)
		g_rl_posy++;
	g_rl_posx = 0;
}
