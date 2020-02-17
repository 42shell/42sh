/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_match.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 08:31:17 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/17 15:05:50 by fratajcz         ###   ########.fr       */
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

/*
** is_first_char means we are at the first char of str (not pat)
*/

bool		is_match(char *str, char *pat, char quote, bool is_first_char)
{
	int			k;

	if (*pat && (quote_start(pat, 0, &quote)))
		return (is_match(str, pat + 1, quote, is_first_char));
	if (is_first_char && *str == '.' && *pat != '.')
		return (false);
	if (*pat == '[' && quote == NONE && (k = match_bracket(*str, pat)))
		return (is_match(str + 1, pat + k, quote, false));
	if (*pat == '*' && quote == NONE)
	{
		return (*str == '\0'
			? is_match(str, pat + 1, quote, is_first_char)
			: is_match(str, pat + 1, quote, is_first_char)
				|| is_match(str + 1, pat, quote, false));
	}
	if (*pat == '?' && quote == NONE)
		return (*str != '\0' && is_match(str + 1, pat + 1, quote, false));
	k = (quote == BSLASH);
	if (*pat && quote_stop(pat, 0, &quote) && !k)
		return (is_match(str, pat + 1, quote, is_first_char));
	if (*str == '\0' && *pat == '\0')
		return (true);
	if (*str && *str == *pat)
		return (is_match(str + 1, pat + 1, quote, false));
	return (false);
}
