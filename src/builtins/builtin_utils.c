/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:58:50 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/07 16:38:45 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

bool	is_valid_var_name(char *str)
{
	if (ft_isdigit(*str))
		return (false);
	while (*str)
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (false);
		str++;
	}
	return (true);
}

bool	is_builtin(char *str)
{
	return (ht_get(g_builtins, str) != NULL);
}
