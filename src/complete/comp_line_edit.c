/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comp_line_edit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/16 15:07:57 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/07 02:43:09 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

bool	y_n_prompt(t_input *input)
{
	u_int64_t c;

	c = 0;
	while (read(0, &c, 8))
	{
		if (c == 'n')
		{
			write(1, "\n", 1);
			draw_prompt(input, input->last_prompt);
			printstr(input->termp, input->line->str);
			return (false);
		}
		if (c == 'y')
			return (true);
		if (c == 3)
		{
			ctrl_c(input);
			return (false);
		}
		c = 0;
	}
	return (false);
}

void		rl_put_match(t_input *input, char *partial, char *match)
{
	char	*diff;
	int		diff_len;

	diff = match + ft_strlen(partial);
	diff_len = ft_strlen(diff);
	while (diff_len--)
		addchar(input, (uint8_t **)&diff);
}

static int	get_max_len(t_list_head *comp_list)
{
	int			max;
	int			len;
	t_list_head	*cur;

	cur = comp_list->next;
	max = 0;
	while (cur != comp_list)
	{
		len = ft_strlen(cur->data);
		if (len > max)
			max = len;
		cur = cur->next;
	}
	return (max);
}

static void	print_list_cols(t_list_head *comp_list, int max_width,
		int skip_len, t_term *term)
{
	int			nb_col;
	int			i;
	t_list_head	*cur;

	max_width += 2;
	nb_col = term->sizex / max_width;
	if (nb_col == 0)
		nb_col = 1;
	cur = comp_list->next;
	while (cur != comp_list)
	{
		i = 0;
		while (i < nb_col && cur != comp_list)
		{
			ft_printf("%-*s", max_width, (char *)cur->data + skip_len);
			cur = cur->next;
			i++;
		}
		ft_printf("\n");
	}
}

/*
** for file completions, the match list has the partial word at the start of
** each match: eg typing libft/<tab> get a match list consisting of libft/src,
** libft/inc, etc.
** we only want to print the last part of the path so we calculate the length
** to skip and add it to every string we print. In the previous example, we
** print src, inc...
*/

extern int g_nb_comp_match;

void		rl_print_match_list(t_list_head *comp_list, char *partial,
		t_input *input)
{
	char		*last_slash;
	int			skip_len;

	if (g_nb_comp_match >= 100)
	{
		ft_printf("\nDisplay all %d possibilities? (y or n)", g_nb_comp_match);
		if (!y_n_prompt(input))
			return ;
	}
	write(1, "\n", 1);
	if ((last_slash = ft_strrchr(partial, '/')) != NULL)
		skip_len = last_slash - partial + 1;
	else
		skip_len = 0;
	print_list_cols(comp_list, get_max_len(comp_list) - skip_len,
			skip_len, input->termp);
	draw_prompt(input, input->last_prompt);
	printstr(input->termp, input->line->str);
}
