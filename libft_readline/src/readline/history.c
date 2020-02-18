/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:08:08 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/16 15:39:06 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readline.h"

int		rl_history_down(void)
{
	if (!g_rl_hist.ptr || g_rl_hist.ptr == g_rl_hist.head)
		return (0);
	rl_move_home();
	ft_dstrdel((void **)&g_rl_line.dstr);
	if ((g_rl_hist.ptr = g_rl_hist.ptr->next) == g_rl_hist.head)
	{
		g_rl_line.dstr = g_rl_line.backup;
		g_rl_line.backup = NULL;
	}
	else if (!(g_rl_line.dstr = ft_dstrfromstr((char *)g_rl_hist.ptr->data)))
	{
		g_rl_error = RL_ALLOC_ERROR;
		return (-1);
	}
	rl_print_line_from_i(false);
	return (0);
}

int		rl_history_up(void)
{
	if (!g_rl_hist.ptr || g_rl_hist.ptr->prev == g_rl_hist.head)
		return (0);
	rl_move_home();
	if (g_rl_hist.ptr == g_rl_hist.head)
		g_rl_line.backup = g_rl_line.dstr;
	else
		ft_dstrdel((void **)&g_rl_line.dstr);
	g_rl_hist.ptr = g_rl_hist.ptr->prev;
	if (!(g_rl_line.dstr = ft_dstrfromstr((char *)g_rl_hist.ptr->data)))
	{
		g_rl_error = RL_ALLOC_ERROR;
		return (-1);
	}
	rl_print_line_from_i(false);
	return (0);
}

int		rl_add_history(char *str)
{
	t_list	*entry;
	char	*dup;

	if (!g_rl_hist.head)
	{
		if (!(g_rl_hist.head = ft_lstnew(NULL)))
		{
			g_rl_error = RL_ALLOC_ERROR;
			return (-1);
		}
		g_rl_hist.ptr = g_rl_hist.head;
	}
	else if ((entry = g_rl_hist.head->prev)
	&& ft_strequ((char *)entry->data, str))
		return (0);
	if (!(dup = ft_strdup(str))
	|| !(entry = ft_lstnew(dup)))
	{
		g_rl_error = RL_ALLOC_ERROR;
		return (-1);
	}
	ft_lstadd(g_rl_hist.head, entry);
	return (0);
}
