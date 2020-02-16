/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_match.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 08:31:17 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/16 09:33:51 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** i == 1 exceptions are there because if ] is the first character, the ']' char
** must match 
*/

static bool	bracket_is_match(char c, char *pat)
{
	int		lowerc;
	int		upperc;	
	int		i;

	i = (pat[1] == '!' || pat[1] == '^') ? 2 : 1;
	while (pat[i] && (pat[i] != ']' || i == 1))
	{
		lowerc = pat[i];
		if (pat[i + 1] == '-')
		{
			i += 2;
			upperc = pat[i];
			if (c >= lowerc && c <= upperc)
				return (true);
		}
		else if (lowerc == c)
			return (true);
		i++;
	}
	return (false);
}

/*
** returns how much pat should advance if the bracket expression in pat
** matches c, 0 if no match
** (is_match != reverse) <=> ((!is_match && reverse) || (is_match && !reverse))
*/

static int	match_bracket(char c, char *pat)
{
	bool	reverse;
	bool	is_match;
	int		i;

	reverse = (pat[1] == '!' || pat[1] == '^');
	is_match = bracket_is_match(c, pat);
	if (is_match != reverse)
	{
		i = 2;
		while (pat[i] && pat[i] != ']')
			i++;
		return (i + 1);
	}
	return (0);
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
