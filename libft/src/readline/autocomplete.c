/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomplete.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:08:08 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/06 11:10:04 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readline.h"

static void	free_comp_list(t_list_head **comp_list)
{
	t_list_head *cur;
	t_list_head *tmp;

	cur = (*comp_list)->next;
	while (cur != *comp_list)
	{
		tmp = cur;
		cur = cur->next;
		free(tmp->data);
		ft_list_del(tmp);
	}
	free(*comp_list);
	*comp_list = NULL;
}

static char	*get_lcp(char *partial_word)
{
	char	*first;
	char	*last;
	int		i;

	ft_list_sort(&g_rl_complete.matches, (t_cmp_func)ft_strcmp);
	first = g_rl_complete.matches->next->data;
	last = g_rl_complete.matches->prev->data;
	i = 0;
	while (first[i] != '\0' && first[i] == last[i])
		i++;
	if (ft_strlen(partial_word) == (unsigned int)i)
		return (NULL);
	return (ft_strndup(first, i));
}

static char	*get_partial_word(void)
{
	int		i;

	i = g_rl_line.i;
	while (i > 0 && g_rl_line.dstr->str[i - 1] != ' ')
		i--;
	return (g_rl_line.dstr->str + i);
}

int			rl_complete(void)
{
	char	*partial_word;
	char	*lcp;

	lcp = NULL;
	if (!g_rl_complete.get_matches)
		return (0);
	partial_word = get_partial_word();
	g_rl_complete.first_tab = g_rl_oldkey == '\t' ? false : true;
	if (g_rl_complete.first_tab && g_rl_complete.matches)
	{
		free_comp_list(&g_rl_complete.matches);
		g_rl_complete.match_count = 0;
	}
	if (!g_rl_complete.matches)
		g_rl_complete.matches = g_rl_complete.get_matches(g_rl_line.dstr->str,
		g_rl_line.dstr->len, &g_rl_complete.match_count);
	if (g_rl_complete.match_count == 0)
		return (0);
	else if (g_rl_complete.match_count > 1 && !g_rl_complete.first_tab)
		rl_print_match_list(partial_word);
	else if ((lcp = get_lcp(partial_word)))
		rl_put_match(lcp, partial_word);
	free(lcp);
	return (0);
}
