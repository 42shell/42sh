/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/07 23:14:17 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/29 15:51:20 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static char		*str(t_list_head *list)
{
	return (((t_dstr *)list->data)->str);
}

static t_dstr	*getnextline(t_input *input)
{
	input->curr = input->curr->next;
	while (input->curr != input->head
	&& ft_strequ(str(input->curr), str(input->curr->prev)))
		input->curr = input->curr->next;
	return ((t_dstr *)input->curr->data);
}

static t_dstr	*getprevline(t_input *input)
{
	input->curr = input->curr->prev;
	while (input->curr->prev != input->head
	&& ft_strequ(str(input->curr), str(input->curr->prev)))
		input->curr = input->curr->prev;
	return ((t_dstr *)input->curr->data);
}

int				history_down(t_input *input)
{
	t_dstr	*target;

	if (input->curr == input->head)
		return (0);
	move_home(input);
	if (input->line)
		ft_dstr_del((void **)&input->line, NULL);
	target = getnextline(input);
	if (input->curr == input->head)
	{
		input->line = input->temp;
		input->temp = NULL;
	}
	else
		input->line = ft_dstr_new(target->str, target->len, target->size);
	clearfromc(input->termp);
	printstr(input->termp, input->line->str);
	input->pos = input->line->len;
	return (0);
}

int				history_up(t_input *input)
{
	t_dstr	*target;

	if (input->curr->prev == input->head)
		return (0);
	move_home(input);
	if (!input->temp)
	{
		input->temp = input->line;
		input->line = NULL;
	}
	else if (input->line)
		ft_dstr_del((void **)&input->line, NULL);
	target = getprevline(input);
	input->line = ft_dstr_new(target->str, target->len, target->size);
	clearfromc(input->termp);
	printstr(input->termp, input->line->str);
	input->pos = input->line->len;
	return (0);
}
