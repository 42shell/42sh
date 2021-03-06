/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_autocomplete_list.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/15 19:22:51 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/25 02:44:13 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	*g_comp_list_count;

t_list_head	*get_autocomplete_list(char *context, size_t i, int *count
									, char *partial_word)
{
	g_comp_list_count = count;
	while (i > 0 && context[i] != ' ')
		i--;
	while (i > 0 && context[i] == ' ')
		i--;
	if (partial_word[0] == '$')
		return (get_variable_list(partial_word));
	if (i <= 0 || context[i] == '|' || context[i] == '&' || context[i] == ';')
	{
		if (ft_strchr(partial_word, '/'))
			return (get_file_list(partial_word, EXECONLY));
		return (comp_get_command_list(partial_word));
	}
	if (partial_word[0] == '-')
		return (get_option_list(partial_word, context, i));
	if (is_cd(context, i))
		return (get_file_list(partial_word, DIRONLY));
	return (get_file_list(partial_word, FILES));
}
