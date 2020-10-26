/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_exp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/05 23:12:16 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/10 18:31:10 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char	*g_expand_error_token = NULL;

/*
** the first char of str should be a $
** Probably should never return -1 since lexer enforces brace termination
*/

bool		is_escaped_brack(const char *str, int i)
{
	return (str[i] == '\\'
		&& (str[i + 1] == '\"' || str[i + 1] == '\'' || str[i + 1] == '\\'
				|| str[i + 1] == '(' || str[i + 1] == '['));
}

int			get_end_of_braces(const char *str, int start)
{
	int		i;
	t_array	*brack_stack;

	brack_stack = array_new(4);
	i = start;
	while (str[i])
	{
		while (is_escaped_brack(str, i))
			i += 2;
		set_bracket_status(str, i, brack_stack, true);
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
	if ((quote_status == SQUOTE && !heredoc) || quote_status == BSLASH)
		return (false);
	if (str[i] == '$'
			|| ((str[i] == '<' || str[i] == '>') && str[i + 1] == '('))
		return (true);
	return (false);
}

static int	do_one_expansion(t_token *token, int *i)
{
	if (token->value->str[*i] == '$')
	{
		if (token->value->str[*i + 1] == '{')
			return (param_expand(token, i, true));
		if (token->value->str[*i + 1] == '(')
		{
			if (token->value->str[*i + 2] == '(')
				return (arith_expand(token, i));
			else
				return (cmd_sub(token, i));
		}
		return (param_expand(token, i, false));
	}
	else
	{
		return (process_sub(token, i));
	}
}

/*
** do all $ expansions such as $(), $(()), $WORD, ${WORD}
** and process substitution :)
*/

int			dollar_expand(t_token *token, int start, bool heredoc)
{
	int		i;
	char	quote_status;
	t_dstr	*str;

	str = token->value;
	ft_memdel((void**)&g_expand_error_token);
	g_expand_error_token = ft_strdup(str->str);
	i = start - 1;
	quote_status = NONE;
	while (str->str[++i])
	{
		if (quote_start(str->str, i, &quote_status))
			continue ;
		if (should_expand(str->str, i, quote_status, heredoc))
			if (do_one_expansion(token, &i) == 1)
				return (1);
		if (i >= 0)
			quote_stop(str->str, i, &quote_status);
	}
	return (0);
}
