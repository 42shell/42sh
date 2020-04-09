/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_autocomplete_list.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/15 19:22:51 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/10 00:23:45 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_list_head	*get_autocomplete_list(char *context, size_t len, int *count)
{
	char	*partial_word;
	int		i;

	i = len;
	g_comp_list_count = count;
	partial_word = &context[i];
	while (i > 0 && context[i] != ' ')
	{
		if (context[--i] != ' ')
			partial_word--;
	}
	while (i > 0 && context[i] == ' ')
		i--;
	if (i <= 0 || context[i] == '|' || context[i] == '&' || context[i] == ';')
	{
		if (ft_strchr(partial_word, '/'))
			return (get_file_list(partial_word, EXECONLY));
		return (comp_get_command_list(partial_word));
	}
	else if (is_cd(context, i))
		return (get_file_list(partial_word, DIRONLY));
	return (get_file_list(partial_word, FILES));
}

