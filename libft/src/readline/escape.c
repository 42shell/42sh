/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   escape.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:08:08 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/02 00:41:14 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readline.h"

static int	rl_get_esc_seq(void)
{
	g_rl_key.i++;
	while (g_rl_key.i < 8)
	{
		if (read(STDIN_FILENO, &g_rl_key.bytes[g_rl_key.i], 1) == -1)
		{
			g_rl_error = RL_SYSCALL_ERROR;
			return (-1);
		}
		if (g_rl_key.i != 1 && g_rl_key.bytes[g_rl_key.i] >= '@')
			break ;
		g_rl_key.i++;
	}
	return (RL_CONTINUE);
}

int			rl_handle_esc_seq(void)
{
	if (rl_get_esc_seq() == -1)
		return (-1);
	else if (*(unsigned long *)g_rl_key.bytes == g_rl_keys[K_LEFT])
		return (rl_move_left());
	else if (*(unsigned long *)g_rl_key.bytes == g_rl_keys[K_RIGHT])
		return (rl_move_right());
	else if (*(unsigned long *)g_rl_key.bytes == g_rl_keys[K_UP])
		return (rl_history_up());
	else if (*(unsigned long *)g_rl_key.bytes == g_rl_keys[K_DOWN])
		return (rl_history_down());
	else if (*(unsigned long *)g_rl_key.bytes == g_rl_keys[K_HOME])
		return (rl_move_home());
	else if (*(unsigned long *)g_rl_key.bytes == g_rl_keys[K_END])
		return (rl_move_end());
	else if (*(unsigned long *)g_rl_key.bytes == g_rl_keys[K_DEL])
		return (rl_delete());
	return (RL_CONTINUE);
}
