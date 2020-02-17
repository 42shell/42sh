/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 18:17:30 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/17 13:14:56 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static bool g_dquote = false;

bool		quote_stop(char *str, int i, char *quote_status)
{
	if (*quote_status == BSLASH)
	{
		*quote_status = g_dquote ? DQUOTE : NONE;
		g_dquote = false;
		return (true);
	}
	if (str[i] != *quote_status || *quote_status == NONE)
		return (false);
	if (*quote_status == DQUOTE && i != 0 && str[i - 1] == BSLASH)
		return (false);
	*quote_status = NONE;
	return (true);
}

bool		quote_start(char *str, int i, char *quote_status)
{
	if (str[i] == BSLASH)
	{
		if (*quote_status == DQUOTE)
			g_dquote = true;
		if (*quote_status == DQUOTE || *quote_status == NONE)
		{
			*quote_status = str[i];
			return (true);
		}
		return (false);
	}
	if ((str[i] == SQUOTE || str[i] == DQUOTE) && *quote_status == NONE)
	{
		*quote_status = str[i];
		return (true);
	}
	return (false);
}

static bool	is_escapable(char c)
{
	return (c == '$' || c == '`' || c == '"' || c == '\\');
}

/*
** We never want to remove a character after a backslash so it's ok to increment
** i directly after we delete a backslash. However, we must be careful not to
** increment i after deleting a backslash at the end of a word.
*/

void		remove_quotes(t_dstr *str)
{
	char	quote_status;
	int		i;
	bool	is_bslash;

	quote_status = NONE;
	i = 0;
	while (str->str[i])
	{
		if (quote_start(str->str, i, &quote_status))
		{
			if (g_dquote && quote_status == BSLASH
					&& !is_escapable(str->str[i + 1]))
				i++;
			else
				ft_dstr_remove(str, i, 1);
			continue;
		}
		is_bslash = (quote_status == BSLASH);
		if (quote_stop(str->str, i, &quote_status))
		{
			is_bslash ? i++ : ft_dstr_remove(str, i, 1);
			continue;
		}
		i++;
	}
}

void		remove_bslash(t_dstr *str)
{
	int i;

	i = 0;
	while (str->str[i])
	{
		if (str->str[i] == '\\')
		{
			if (str->str[i + 1] == '\n')
			{
				ft_dstr_remove(str, i, 2);
				continue;
			}
			if (str->str[i + 1] == '$' || str->str[i + 1] == '\\')
				ft_dstr_remove(str, i, 1);
		}
		i++;
	}
}
