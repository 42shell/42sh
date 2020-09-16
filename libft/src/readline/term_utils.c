/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:08:08 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/25 02:54:44 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readline.h"

size_t	g_rl_sizex;
size_t	g_rl_sizey;
size_t	g_rl_posx;
size_t	g_rl_posy;

int		get_cpos(void)
{
	char	buf[16];
	char	*p;

	p = buf;
	ft_bzero(buf, 16);
	if (write(STDIN_FILENO, "\033[6n", 4) != 4
	|| read(STDIN_FILENO, buf, 15) < 0)
	{
		g_rl_error = RL_SYSCALL_ERROR;
		return (-1);
	}
	g_rl_posy = ft_atoi(p + 2) - 1;
	while (*p && *p != ';')
		p++;
	g_rl_posx = ft_atoi(p + 1) - 1;
	return (0);
}

int		get_winsize(void)
{
	struct winsize	win;

	if (ioctl(STDIN_FILENO, TIOCGWINSZ, &win) == -1)
	{
		g_rl_sizex = tgetnum("li");
		g_rl_sizey = tgetnum("co");
	}
	else
	{
		g_rl_sizex = win.ws_col;
		g_rl_sizey = win.ws_row;
	}
	if (g_rl_sizex == 0 || g_rl_sizey == 0)
	{
		g_rl_error = RL_TERM_ERROR;
		return (-1);
	}
	return (0);
}
