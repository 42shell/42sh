/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:08:08 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/25 02:53:41 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readline.h"

struct termios	g_rl_term;
struct termios	g_rl_oldterm;

static void	init_caps(void)
{
	g_rl_caps[C_UP] = tgetstr("up", NULL);
	g_rl_caps[C_DO] = tgetstr("do", NULL);
	g_rl_caps[C_LE] = tgetstr("le", NULL);
	g_rl_caps[C_ND] = tgetstr("nd", NULL);
	g_rl_caps[C_HO] = tgetstr("ho", NULL);
	g_rl_caps[C_CH] = tgetstr("ch", NULL);
	g_rl_caps[C_CE] = tgetstr("ce", NULL);
	g_rl_caps[C_CD] = tgetstr("cd", NULL);
	g_rl_caps[C_DC] = tgetstr("dc", NULL);
	g_rl_caps[C_SC] = tgetstr("sc", NULL);
	g_rl_caps[C_RC] = tgetstr("rc", NULL);
	g_rl_caps[C_CR] = tgetstr("cr", NULL);
	g_rl_caps[C_SF] = tgetstr("sf", NULL);
	g_rl_caps[C_SR] = tgetstr("sr", NULL);
	g_rl_caps[C_CM] = tgetstr("cm", NULL);
	g_rl_caps[C_CL] = tgetstr("cl", NULL);
	g_rl_caps[C_BL] = tgetstr("bl", NULL);
	g_rl_caps[C_KS] = tgetstr("ks", NULL);
	tputs(g_rl_caps[C_KS], 1, ft_putc);
}

int			rl_init_term(void)
{
	if (tgetent(NULL, getenv("TERM")) != 1)
	{
		ft_putstr_fd("42sh: Could not find your terminal in terminfo database"
						", try setting $TERM\n", 2);
		exit(1);
	}
	tcgetattr(STDIN_FILENO, &g_rl_oldterm);
	g_rl_term = g_rl_oldterm;
	g_rl_term.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	g_rl_term.c_cc[VMIN] = 1;
	g_rl_term.c_cc[VTIME] = 0;
	if (get_winsize() == -1)
		return (-1);
	init_caps();
	return (0);
}
