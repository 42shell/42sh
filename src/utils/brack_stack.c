/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brack_stack.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/05 22:26:59 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/08 16:25:24 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static bool		g_just_closed_dparen;

enum e_bracket	get_bracket_status(t_array *stack)
{
	if (stack->size == 0)
		return ('\0');
	return (*(enum e_bracket *)stack->array[stack->size - 1]);
}

void			add_bracket_to_stack(t_array *stack, enum e_bracket bracket)
{
	enum e_bracket	*data;

	data = malloc(sizeof(enum e_bracket));
	*data = bracket;
	array_append(stack, data);
}

void			pop_bracket_from_stack(t_array *stack)
{
	enum e_bracket	*bracket;

	bracket = array_pop(stack);
	if (*bracket == DPAREN)
		g_just_closed_dparen = true;
	free(bracket);
}

/*
** in case of $(()) inside a $(), we don't want to close the $() with the second** paren of the )) that close the $(( so we use g_just_closed_dparen.
** flags: BRACK_CAN_OPEN, BRACK_CAN_CLOSE.
*/

void			set_bracket_status(char *str, int i, t_array *stack, int flags)
{
	enum e_bracket	brack_status;

	brack_status = get_bracket_status(stack);
	if ((flags & BRACK_CAN_OPEN) && str[i] == '$')
	{
		if (str[i + 1] == '{')
			add_bracket_to_stack(stack, BRACE);
		else if (str[i + 1] == '(')
			add_bracket_to_stack(stack, str[i + 2] == '(' ? DPAREN : PAREN);
	}
	if (flags & BRACK_CAN_CLOSE)
	{
		if (brack_status == BRACE && str[i] == '}')
			pop_bracket_from_stack(stack);
		else if (brack_status == DPAREN && str[i] == ')'
				&& str[i + 1] == ')' && !g_just_closed_dparen)
			return (pop_bracket_from_stack(stack));
		else if (brack_status == PAREN && str[i] == ')' &&
				!g_just_closed_dparen)
			pop_bracket_from_stack(stack);
	}
	g_just_closed_dparen = false;
}
