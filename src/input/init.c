/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/06 01:23:20 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/07 02:44:08 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_key_func	*init_functable(void)
{
	t_key_func	*table;

	table = ft_xmalloc(sizeof(t_key_func) * 32);
	table[10] = &enter;
	table[14] = &move_nextword;
	table[24] = &move_prevword;
	table[21] = &cut_before;
	table[11] = &cut_after;
	table[23] = &cut_word;
	table[25] = &paste;
	table[12] = &redraw;
	table[3] = &ctrl_c;
	table[4] = &ctrl_d;
	table[9] = &rl_complete;
	return (table);
}

static void	init_keycaps(struct s_input *input)
{
	char	*str;

	if ((str = tgetstr("kD", NULL)))
		input->keys[K_DEL] = *(int *)str;
	if ((str = tgetstr("kl", NULL)))
		input->keys[K_LEFT] = *(int *)str;
	if ((str = tgetstr("kr", NULL)))
		input->keys[K_RIGHT] = *(int *)str;
	if ((str = tgetstr("ku", NULL)))
		input->keys[K_UP] = *(int *)str;
	if ((str = tgetstr("kd", NULL)))
		input->keys[K_DOWN] = *(int *)str;
	if ((str = tgetstr("kh", NULL)))
		input->keys[K_HOME] = *(int *)str;
}

static void	init_keys(struct s_input *input)
{
	init_keycaps(input);
	input->keys[K_END] = 4607771;
	input->keys[K_BSP] = 127;
	input->keys[K_ESC] = 27;
	input->keys[K_SPC] = 32;
	input->keys[K_ENTER] = 10;
	input->keys[K_REDRAW] = 12;
	input->keys[K_PRVW] = 24;
	input->keys[K_NXTW] = 14;
	input->keys[K_CUTW] = 23;
	input->keys[K_CUTA] = 11;
	input->keys[K_CUTB] = 21;
	input->keys[K_PAST] = 25;
}

int			init_input(struct s_input *input, struct s_term *term)
{
	if (input->interactive)
	{
		init_keys(input);
		input->termp = term;
		input->head = ft_list_first_head(NULL);
		input->curr = input->head;
		input->complete = true;
	}
	input->first_line = true;
	input->first_tab_press = true;
	return (0);
}
