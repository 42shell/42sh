/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_basic.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/06 01:18:08 by fratajcz          #+#    #+#             */
/*   Updated: 2019/12/06 01:18:56 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		move_left(t_input *input)
{
	if (input->pos > input->pos_min)
	{
		input->pos -= ft_charlen_rev(&input->line->str[input->pos - 1]);
		movcleft(input->termp);
	}
	return (0);
}

int		move_right(t_input *input)
{
	if (input->pos < input->line->len)
	{
		input->pos += ft_charlen(input->line->str[input->pos]);
		movcright(input->termp);
	}
	return (0);
}

int		move_up(t_input *input)
{
	size_t	offset;
	size_t	i;

	offset = 0;
	i = input->termp->sizex;
	while (i-- && input->pos > input->pos_min)
	{
		input->pos -= ft_charlen_rev(&input->line->str[input->pos - 1]);
		offset++;
	}
	movcto(input->termp, input->termp->cpos - offset);
	return (0);
}

int		move_down(t_input *input)
{
	size_t	offset;
	size_t	i;

	offset = 0;
	i = input->termp->sizex;
	while (i-- && input->pos < input->line->len)
	{
		input->pos += ft_charlen(input->line->str[input->pos]);
		offset++;
	}
	movcto(input->termp, input->termp->cpos + offset);
	return (0);
}

