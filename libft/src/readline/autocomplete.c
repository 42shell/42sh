/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomplete.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:08:08 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/25 03:00:04 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readline.h"

struct s_rl_complete g_rl_complete;

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
	g_rl_complete.match_count = 0;
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
	return (ft_strndup(g_rl_line.dstr->str + i, g_rl_line.i - i));
}

bool		is_same_word(char *partial_word)
{
	bool same_word;

	same_word = ft_strequ(g_rl_complete.old_partial_word, partial_word);
	free(g_rl_complete.old_partial_word);
	g_rl_complete.old_partial_word = ft_strdup(partial_word);
	if (g_rl_oldkey != '\t')
		return (false);
	return (same_word);
}

int			rl_complete(void)
{
	char	*partial_word;
	char	*lcp;
	bool	same_word;

	lcp = NULL;
	partial_word = get_partial_word();
	same_word = is_same_word(partial_word);
	if (!same_word && g_rl_complete.matches)
		free_comp_list(&g_rl_complete.matches);
	if (!g_rl_complete.matches)
		g_rl_complete.matches = g_rl_complete.get_matches(g_rl_line.dstr->str,
		g_rl_line.i, &g_rl_complete.match_count, partial_word);
	if (g_rl_complete.match_count == 0)
	{
		free(partial_word);
		return (0);
	}
	else if (g_rl_complete.match_count > 1 && same_word)
		rl_print_match_list(partial_word);
	else if ((lcp = get_lcp(partial_word)))
		rl_put_match(lcp, partial_word);
	free(lcp);
	free(partial_word);
	return (0);
}
