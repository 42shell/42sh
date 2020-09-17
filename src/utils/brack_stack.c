/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brack_stack.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/05 22:26:59 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/17 14:14:23 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

enum e_quote_st	get_bracket_status(t_array *stack)
{
	if (stack->size == 0)
		return ('\0');
	return (*(enum e_quote_st *)stack->array[stack->size - 1]);
}

void			add_bracket_to_stack(t_array *stack, enum e_quote_st bracket)
{
	enum e_quote_st	*data;

	data = ft_xmalloc(sizeof(enum e_quote_st));
	*data = bracket;
	array_append(stack, data);
}

void			pop_bracket_from_stack(t_array *stack)
{
	enum e_quote_st	*bracket;

	bracket = array_pop(stack);
	free(bracket);
}

static bool		should_pop(const char *str, int i, enum e_quote_st brack_status
						, bool just_closed_dparen)
{
	return (
	(brack_status == BRACE && str[i] == '}')
	|| (brack_status == DPAREN && str[i] == ')' && i > 0 && str[i - 1] == ')'
				&& !just_closed_dparen)
	|| (brack_status == PAREN && str[i] == ')' && !just_closed_dparen)
	|| ((str[i] == DQUOTE || str[i] == SQUOTE)
				&& (char)brack_status == str[i]));
}

/*
** in case of $(()) inside a $(), we don't want to close the $() with the second
** paren of the )) that close the $(( so we use g_just_closed_dparen.
** flags: BRACK_CAN_OPEN, BRACK_CAN_CLOSE.
*/

void			set_bracket_status(const char *str, int i, t_array *stack,
									bool can_open)
{
	enum e_quote_st	brack_status;
	static bool		just_closed_dparen;

	brack_status = get_bracket_status(stack);
	if (can_open && str[i] == '$')
	{
		if (str[i + 1] == '{')
			add_bracket_to_stack(stack, BRACE);
		else if (str[i + 1] == '(')
			add_bracket_to_stack(stack, str[i + 2] == '(' ? DPAREN : PAREN);
	}
	else if (can_open && (str[i] == DQUOTE || str[i] == SQUOTE)
			&& brack_status != SQUOTE && brack_status != DQUOTE)
		add_bracket_to_stack(stack, str[i]);
	else if (should_pop(str, i, brack_status, just_closed_dparen))
	{
		just_closed_dparen = (brack_status == DPAREN);
		pop_bracket_from_stack(stack);
		if (just_closed_dparen)
			return ;
	}
	just_closed_dparen = false;
}
