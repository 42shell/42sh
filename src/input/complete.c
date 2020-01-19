/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complete.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/15 19:22:51 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/17 13:54:33 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		g_nb_comp_match;
extern	t_env	*g_env;

/*
** get the lowest common prefix of the list of matches. We sort the list then
** look at the lcp of the first and last elements of the list to find the lcp
** of the full list.
*/

typedef	int	(*t_cmp_func) (void *a, void *b);

char		*get_lcp(t_list_head *comp_list, char *partial)
{
	char	*first;
	char	*last;
	int		i;

	ft_list_sort(&comp_list, (t_cmp_func)ft_strcmp);
	first = comp_list->next->data;
	last = comp_list->prev->data;
	i = 0;
	while (first[i] == last[i])
		i++;
	if (ft_strlen(partial) == (unsigned int)i)
		return (NULL);
	return (ft_strndup(first, i));
}

char		*comp_get_partial_word(t_input *input)
{
	int		i;

	i = input->pos;
	while (i > 0 && input->line->str[i - 1] != ' ')
		i--;
	return (ft_strndup(input->line->str + i, input->pos - i));
}

t_list_head	*comp_get_list(t_input *input, char *partial)
{
	int i;

	i = input->pos;
	while (i > 0 && input->line->str[i] != ' ')
		i--;
	while (i > 0 && input->line->str[i] == ' ')
		i--;
	if (i == 0 || input->line->str[i] == '|' || input->line->str[i] == ';')
		return (comp_get_command_list(partial, g_env));
	else if (is_cd(input->line->str, i))
		return (comp_get_file_list(partial, true));
	else
		return (comp_get_file_list(partial, false));
}

int			rl_complete(t_input *input)
{
	t_list_head *comp_list;
	char		*lcp;
	char		*partial;

	if (input->line->str[input->pos] != '\0'
			&& input->line->str[input->pos] != ' ')
		return (0);
	partial = comp_get_partial_word(input);
	comp_list = comp_get_list(input, partial);
	lcp = NULL;
	if (g_nb_comp_match == 1)
		rl_put_match(input, partial, comp_list->next->data);
	else if (g_nb_comp_match > 1)
	{
		if (input->first_tab_press && (lcp = get_lcp(comp_list, partial)))
			rl_put_match(input, partial, lcp);
		if (!input->first_tab_press)
			rl_print_match_list(comp_list, input);
		input->first_tab_press = (lcp == NULL) ? !input->first_tab_press : true;
		free(lcp);
	}
	free(partial);
	free_comp_list(comp_list);
	return (0);
}
