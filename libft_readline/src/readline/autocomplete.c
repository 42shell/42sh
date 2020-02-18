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

int		rl_put_match(char *partial, char *match)
{
	(void)partial;
	(void)match;
	return (0);
}

int		rl_print_match_list(t_list *comp_list, char *partial)
{
	(void)comp_list;
	(void)partial;
	return (0);
}

int		rl_complete(void)
{
	char	**matches;

	if (!g_rl_get_matches)
		return (0);
	if (!(matches = g_rl_get_matches(g_rl_line.dstr->str)))
		return (0);
	else if (matches[1] == NULL)
		return (rl_put_match(NULL, NULL));
	else
		return (rl_print_match_list(NULL, NULL));
	return (0);
}
