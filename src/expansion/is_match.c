/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_match.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 08:31:17 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/17 14:43:50 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static int g_i;

/*
** i == 1 exceptions are there because if ] is the first character, the ']' char
** must match
*/

static bool	bracket_is_match(char c, char *pat)
{
	int		lowerc;
	int		upperc;

	g_i = (pat[1] == '!' || pat[1] == '^') ? 2 : 1;
	while (pat[g_i] && (pat[g_i] != ']' || g_i == 1))
	{
		lowerc = pat[g_i];
		if (pat[g_i + 1] == '-')
		{
			g_i += 2;
			upperc = pat[g_i];
			if (c >= lowerc && c <= upperc)
				return (true);
		}
		else if (lowerc == c)
			return (true);
		g_i++;
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

	reverse = (pat[1] == '!' || pat[1] == '^');
	is_match = bracket_is_match(c, pat);
	if (is_match != reverse)
	{
		while (pat[g_i] && pat[g_i] != ']')
			g_i++;
		return (g_i + 1);
	}
	return (0);
}

bool		is_match(char *str, int i, char *pat, int j, char quote)
{
	int			k;

	if (i == 0 && j == 0)
		quote = NONE;
	if (pat[j] && (quote_start(pat, j, &quote)))
		return (is_match(str, i, pat, j + 1, quote));
	if (i == 0 && str[0] == '.' && pat[j] != '.')
		return (false);
	if (pat[j] == '[' && quote == NONE && (k = match_bracket(str[i], &pat[j])))
		return (is_match(str, i + 1, pat, j + k, quote));
	if (pat[j] == '*' && quote == NONE)
	{
		return (str[i] == '\0'
			? is_match(str, i, pat, j + 1, quote)
			: is_match(str, i, pat, j + 1, quote) || is_match(str, i + 1, pat, j, quote));
	}
	if (pat[j] == '?' && quote == NONE)
		return (str[i] != '\0' && is_match(str, i + 1, pat, j + 1, quote));
	k = (quote == BSLASH);
	if (pat[j] && quote_stop(pat, j, &quote) && !k)
		return (is_match(str, i, pat, j + 1, quote));
	if (str[i] == '\0' && pat[j] == '\0')
		return (true);
	if (str[i] && str[i] == pat[j])
		return (is_match(str, i + 1, pat, j + 1, quote));
	return (false);
}
