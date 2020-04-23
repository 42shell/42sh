/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interrupt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:08:08 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/02 00:41:39 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readline.h"

int		rl_interrupt(void)
{
	ft_dstr_clear(g_rl_line.dstr, 32);
	g_rl_line.i = 0;
	write(1, "^C", 2);
	return (RL_INTERRUPT);
}

int		rl_eof(void)
{
	if (g_rl_line.dstr->len == 0)
		return (RL_EOF);
	return (RL_CONTINUE);
}
