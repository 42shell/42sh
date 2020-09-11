/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomplete_display.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:08:08 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 18:46:27 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readline.h"
#include "ft_printf.h"

static int	get_max_len(void)
{
	int			max;
	int			len;
	t_list_head	*cur;

	cur = g_rl_complete.matches->next;
	max = 0;
	while (cur != g_rl_complete.matches)
	{
		len = ft_strlen(cur->data);
		if (len > max)
			max = len;
		cur = cur->next;
	}
	return (max);
}

static void	print_list_cols(int max_width, int skip_len)
{
	int			nb_col;
	int			i;
	t_list_head	*cur;

	max_width += 2;
	nb_col = g_rl_sizex / max_width;
	if (nb_col == 0)
		nb_col = 1;
	cur = g_rl_complete.matches->next;
	while (cur != g_rl_complete.matches)
	{
		i = 0;
		while (i < nb_col && cur != g_rl_complete.matches)
		{
			ft_printf("%-*s", max_width, (char *)cur->data + skip_len);
			cur = cur->next;
			i++;
		}
		movcnl();
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

void		rl_print_match_list(char *partial_word)
{
	char		*last_slash;
	int			skip_len;
	size_t		old_pos;

	movcnl();
	if (g_rl_complete.match_count >= 100)
	{
		ft_printf("Display all %d possibilities? (y or n)"
				, g_rl_complete.match_count);
		if (!y_n_prompt())
			return ;
		movcnl();
	}
	if ((last_slash = ft_strrchr(partial_word, '/')) != NULL)
		skip_len = last_slash - partial_word + 1;
	else
		skip_len = 0;
	print_list_cols(get_max_len() - skip_len, skip_len);
	old_pos = g_rl_line.i;
	rl_print_prompt(g_rl_prompt);
	rl_print_line(false);
	while (g_rl_line.i != old_pos)
		rl_move_left();
}

void		rl_put_match(char *match, char *partial_word)
{
	struct s_rl_key	tmp;
	char			*diff;
	int				diff_len;

	tmp = g_rl_key;
	ft_bzero(&g_rl_key, sizeof(g_rl_key));
	diff = match + ft_strlen(partial_word);
	diff_len = ft_strlen(diff);
	while (diff_len--)
	{
		g_rl_key.bytes[0] = *diff++;
		rl_insert_char();
	}
	g_rl_key = tmp;
}
