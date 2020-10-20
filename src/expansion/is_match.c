/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_match.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 08:31:17 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/17 04:39:33 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static int g_i;

static int	get_bracket_end(char *pat)
{
	char	quote_status;
	int		i;

	quote_status = NONE;
	i = 0;
	while (pat[++i])
	{
		if (quote_start(pat, i, &quote_status))
			continue;
		if (quote_status == NONE && pat[i] == ']' && i != 1)
			return (i);
		quote_stop(pat, i, &quote_status);
	}
	return (0);
}

/*
** i == 1 exceptions are there because if ] is the first character, the ']' char
** must match
*/

static bool	bracket_is_match(char c, char *pat, int bracket_end)
{
	char	quote_status;
	int		lowerc;
	int		upperc;

	quote_status = NONE;
	g_i = (pat[1] == '!' || pat[1] == '^') ? 1 : 0;
	while (++g_i < bracket_end)
	{
		if (quote_start(pat, g_i, &quote_status))
			continue;
		lowerc = pat[g_i];
		if (pat[g_i + 1] == '-' && pat[g_i + 2] != ']' && quote_status == NONE)
		{
			g_i += 2;
			upperc = pat[g_i];
			if (c >= lowerc && c <= upperc)
				return (true);
		}
		else if (lowerc == c
				&& (lowerc != quote_status || quote_status == BSLASH))
			return (true);
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
	int		bracket_end;

	if ((bracket_end = get_bracket_end(pat)) == 0)
		return (-1);
	reverse = (pat[1] == '!' || pat[1] == '^');
	is_match = bracket_is_match(c, pat, bracket_end);
	if (is_match != reverse)
		return (bracket_end + 1);
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
	if (*pat == '[' && quote == NONE && (k = match_bracket(*str, pat)) >= 0)
		return (k == 0 ? false : is_match(str + 1, pat + k, quote, false));
	if (*pat == '*' && quote == NONE)
	{
		while (*(pat + 1) == '*')
			pat++;
		return (is_match(str, pat + 1, quote, is_first_char)
				|| (*str != '\0' ? is_match(str + 1, pat, quote, false) : 0));
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
