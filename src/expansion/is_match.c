/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_match.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 08:31:17 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/16 08:32:15 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** returns how much pat should advance if the bracket expression in pat matches,
** 0 if no match
** i == 1 exceptions are there because if ] is the first character, the ']' char
** must match 
*/

static int	match_bracket(char c, char *pat)
{
	int		lowerc;
	int		upperc;	
	int		i;

	i = 1;
	while (pat[i] && (pat[i] != ']' || i == 1))
	{
		lowerc = pat[i];
		if (pat[i + 1] == '-')
		{
			i += 2;
			upperc = pat[i];
			if (c >= lowerc && c <= upperc)
				break ;
		}
		else if (lowerc == c)
			break ;
		i++;
	}
	if (pat[i] == ']' && i != 1)
		return (0);
	while (pat[i] && (pat[i] != ']' || i == 1))
		i++;
	return (pat[i] == '\0' ? 0 : i + 1);
}

bool		is_match(char *str, int i, char *pat, int j)
{
	static char quote;
	int			k;

	if (i == 0 && j == 0)
		quote = NONE;
	if (i == 0 && str[0] == '.' && quote == NONE
			&& (pat[j] == '*' || pat[j] == '?'))
		return (false);
	if (str[i] && (quote_start(pat, j, &quote) || quote_stop(pat, j, &quote)))
		return (is_match(str, i + 1, pat, j));
	if (pat[j] == '[' && quote == NONE && (k = match_bracket(str[i], &pat[j])))
		return (is_match(str, i + 1, pat, j + k));
	if (pat[j] == '*' && quote == NONE)
		return (str[i] == '\0'
			? is_match(str, i, pat, j + 1)
			: is_match(str, i, pat, j + 1)  || is_match(str, i + 1, pat, j));
	if (pat[j] == '?' && quote == NONE)
		return (str[i] != '\0' && is_match(str, i + 1, pat, j + 1));
	if (str[i] == '\0' && pat[j] == '\0')
		return (true);
	if (str[i] && str[i] == pat[j])
		return (is_match(str, i + 1, pat, j + 1));
	return (false);
}
