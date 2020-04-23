/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binding.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:08:08 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/02 00:38:13 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readline.h"

static void	init_keycaps(void)
{
	char	*str;

	if ((str = tgetstr("kD", NULL)))
		g_rl_keys[K_DEL] = *(int *)str;
	if ((str = tgetstr("kl", NULL)))
		g_rl_keys[K_LEFT] = *(int *)str;
	if ((str = tgetstr("kr", NULL)))
		g_rl_keys[K_RIGHT] = *(int *)str;
	if ((str = tgetstr("ku", NULL)))
		g_rl_keys[K_UP] = *(int *)str;
	if ((str = tgetstr("kd", NULL)))
		g_rl_keys[K_DOWN] = *(int *)str;
	if ((str = tgetstr("kh", NULL)))
		g_rl_keys[K_HOME] = *(int *)str;
}

static void	init_keys(void)
{
	init_keycaps();
	g_rl_keys[K_END] = 4607771;
	g_rl_keys[K_BSP] = 127;
	g_rl_keys[K_ESC] = 27;
	g_rl_keys[K_ENTER] = 10;
	g_rl_keys[K_REDRAW] = 12;
	g_rl_keys[K_PRVW] = 24;
	g_rl_keys[K_NXTW] = 14;
	g_rl_keys[K_CUTW] = 23;
	g_rl_keys[K_CUTA] = 11;
	g_rl_keys[K_CUTB] = 21;
	g_rl_keys[K_PAST] = 25;
	g_rl_keys[K_CTRLC] = 3;
	g_rl_keys[K_CTRLD] = 4;
	g_rl_keys[K_TAB] = 9;
}

static void	bind_controls(void)
{
	g_rl_keymap[g_rl_keys[K_CTRLC]] = &rl_interrupt;
	g_rl_keymap[g_rl_keys[K_CTRLD]] = &rl_eof;
	g_rl_keymap[g_rl_keys[K_ENTER]] = &rl_enter;
	g_rl_keymap[g_rl_keys[K_BSP]] = &rl_backspace;
	g_rl_keymap[g_rl_keys[K_ESC]] = &rl_handle_esc_seq;
	g_rl_keymap[g_rl_keys[K_PRVW]] = &rl_move_prevw;
	g_rl_keymap[g_rl_keys[K_NXTW]] = &rl_move_nextw;
	g_rl_keymap[g_rl_keys[K_CUTB]] = &rl_cut_before;
	g_rl_keymap[g_rl_keys[K_CUTA]] = &rl_cut_after;
	g_rl_keymap[g_rl_keys[K_CUTW]] = &rl_cut_word;
	g_rl_keymap[g_rl_keys[K_PAST]] = &rl_paste;
	g_rl_keymap[g_rl_keys[K_TAB]] = &rl_complete;
}

static int	rl_ignore_key(void)
{
	return (RL_CONTINUE);
}

int			rl_bind_keys(void)
{
	int		i;

	i = 0;
	init_keys();
	bind_controls();
	while (i < 255)
	{
		if (!g_rl_keymap[i])
		{
			if (i < 32 || i > 127)
				g_rl_keymap[i] = &rl_ignore_key;
			else
				g_rl_keymap[i] = &rl_insert_char;
		}
		i++;
	}
	return (0);
}
