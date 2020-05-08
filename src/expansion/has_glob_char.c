/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   has_glob_char.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/08 18:27:39 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 18:27:41 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

bool	has_glob_char(char *str)
{
	char	quote_status;
	bool	bracket_open;
	int		i;

	quote_status = NONE;
	i = -1;
	while (str[++i])
	{
		if (quote_start(str, i, &quote_status))
			continue ;
		if (quote_status == NONE)
		{
			if (str[i] == '*' || str[i] == '?')
				return (true);
			if (str[i] == '[' && !bracket_open)
				bracket_open = true;
			if (str[i] == ']' && bracket_open)
				return (true);
		}
		quote_stop(str, i, &quote_status);
	}
	return (false);
}
