/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copypaste.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/06 01:18:08 by fratajcz          #+#    #+#             */
/*   Updated: 2019/12/06 01:18:56 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readline.h"

static int	rl_append_word(size_t len)
{
	if (ft_dstrinsert(g_rl_line.clipboard, 0,
						&g_rl_line.dstr->str[g_rl_line.i], len) == -1)
	{
		g_rl_error = RL_ALLOC_ERROR;
		return (-1);
	}
	ft_dstrremove(g_rl_line.dstr, g_rl_line.i, len);
	rl_print_line_from_i(true);
	return (0);
}

int			rl_cut_word(void)
{
	size_t	len;

	if ((len = g_rl_line.i) == 0
	|| (rl_move_prevw() == 0 && (len -= g_rl_line.i) == 0))
		return (0);
	if (g_rl_oldkey == g_rl_keys[K_CUTW] && g_rl_line.clipboard != NULL)
		return (rl_append_word(len));
	else if (g_rl_line.clipboard != NULL)
		ft_dstrdel((void **)&g_rl_line.clipboard);
	if (!(g_rl_line.clipboard = ft_dstrnew(len)))
	{
		g_rl_error = RL_ALLOC_ERROR;
		return (-1);
	}
	ft_memcpy(g_rl_line.clipboard->str, &g_rl_line.dstr->str[g_rl_line.i], len);
	g_rl_line.clipboard->len = len;
	ft_dstrremove(g_rl_line.dstr, g_rl_line.i, len);
	rl_print_line_from_i(true);
	return (0);
}

int			rl_cut_after(void)
{
	size_t	len;

	if ((len = g_rl_line.dstr->len - g_rl_line.i) == 0)
		return (0);
	if (g_rl_line.clipboard != NULL)
		ft_dstrdel((void **)&g_rl_line.clipboard);
	if (!(g_rl_line.clipboard = ft_dstrnew(len)))
	{
		g_rl_error = RL_ALLOC_ERROR;
		return (-1);
	}
	ft_memcpy(g_rl_line.clipboard->str, &g_rl_line.dstr->str[g_rl_line.i], len);
	g_rl_line.clipboard->len = len;
	ft_dstrremove(g_rl_line.dstr, g_rl_line.i, len);
	rl_print_line_from_i(true);
	return (0);
}

int			rl_cut_before(void)
{
	size_t	len;

	if ((len = g_rl_line.i) == 0)
		return (0);
	if (g_rl_line.clipboard != NULL)
		ft_dstrdel((void **)&g_rl_line.clipboard);
	if (!(g_rl_line.clipboard = ft_dstrnew(len)))
	{
		g_rl_error = RL_ALLOC_ERROR;
		return (-1);
	}
	ft_memcpy(g_rl_line.clipboard->str, g_rl_line.dstr->str, len);
	g_rl_line.clipboard->len = len;
	rl_move_home();
	ft_dstrremove(g_rl_line.dstr, 0, len);
	rl_print_line_from_i(true);
	return (0);
}

int			rl_paste(void)
{
	size_t	p_len;
	size_t	i;

	i = 0;
	if (g_rl_line.clipboard == NULL)
		return (0);
	p_len = g_rl_line.clipboard->len;
	if (g_rl_line.clipboard->len + p_len > g_rl_line_size_max
	&& !(p_len = g_rl_line_size_max - g_rl_line.dstr->len))
		return (0);
	else if (ft_dstrinsert(g_rl_line.dstr, g_rl_line.i,
	g_rl_line.clipboard->str, p_len) == -1)
	{
		g_rl_error = RL_ALLOC_ERROR;
		return (-1);
	}
	rl_print_line_from_i(true);
	while (i++ < p_len)
		rl_move_right();
	return (0);
}
