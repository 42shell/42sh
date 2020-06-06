/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_exp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/05 23:12:16 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/06 16:21:21 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char	*g_expand_error_token = NULL;

/*
** the first char of str should be a $
** Probably should never return -1 since lexer enforces brace termination
*/

int			get_end_of_braces(const char *str)
{
	int		i;
	t_array	*brack_stack;
	int		flags;

	brack_stack = array_new(4);
	flags = BRACK_CAN_OPEN | BRACK_CAN_CLOSE;
	i = 0;
	while (str[i])
	{
		set_bracket_status(str, i, brack_stack, flags);
		if (brack_stack->size == 0)
		{
			array_destroy(brack_stack);
			return (i);
		}
		i++;
	}
	array_destroy(brack_stack);
	return (-1);
}

static bool	should_expand(char *str, int i, char quote_status, bool heredoc)
{
	if (str[i] != '$')
		return (false);
	if ((quote_status == SQUOTE && !heredoc) || quote_status == BSLASH)
		return (false);
	return (true);
}

static int	do_one_expansion(t_dstr *str, int *i)
{
	if (str->str[*i + 1] == '{')
		return (param_expand(str, i, true));
	return (param_expand(str, i, false));
}

int			dollar_expand(t_dstr *str, int start, bool heredoc)
{
	int		i;
	char	quote_status;

	ft_memdel((void**)&g_expand_error_token);
	g_expand_error_token = ft_strdup(str->str);
	i = start - 1;
	quote_status = NONE;
	while (str->str[++i])
	{
		if (quote_start(str->str, i, &quote_status))
			continue ;
		if (should_expand(str->str, i, quote_status, heredoc))
			if (do_one_expansion(str, &i) == 1)
				return (1);
		if (i >= 0)
			quote_stop(str->str, i, &quote_status);
	}
	return (0);
}
