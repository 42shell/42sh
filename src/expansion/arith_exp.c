/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arith_exp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/06 15:58:15 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/17 14:15:46 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

extern bool g_arith_toomuch;

/*
** Only dquotes and backslashes in front of dquotes are removed. why? idk
*/

static void	remove_one_char(t_token *token, int i)
{
	if (token->exp_info)
		ft_dstr_remove(token->exp_info, i, 1);
	ft_dstr_remove(token->value, i, 1);
}

static void	remove_dquotes(t_token *token, int start)
{
	int		i;
	t_array	*brack_stack;

	brack_stack = array_new(4);
	i = start;
	while (token->value->str[i])
	{
		while (is_escaped_brack(token->value->str, i)
				&& token->value->str[i + 1] != '"')
			i += 2;
		set_bracket_status(token->value->str, i, brack_stack, true);
		if (brack_stack->size == 0
				|| (brack_stack->size == 1
					&& get_bracket_status(brack_stack) == DQUOTE))
		{
			if (token->value->str[i] == '\\' && token->value->str[i + 1] == '"')
				remove_one_char(token, i++);
			else if (token->value->str[i] == '"')
				remove_one_char(token, i);
		}
		i++;
	}
	array_destroy(brack_stack);
}

#define ERR 1

extern int g_arith_status;

/*
** get_end of braces returns the index of the second closing parenthesis, so we
** need to substract 1 to end
*/

int			arith_expand(t_token *token, int *i)
{
	int		end;
	char	*buf;

	end = get_end_of_braces(token->value->str, *i) - 1;
	token->value->str[end] = '\0';
	token->exp_info->str[end] = '\0';
	remove_dquotes(token, *i + 3);
	if (dollar_expand(token, *i + 3, false) == 1)
		return (1);
	end = ft_strlen(token->value->str);
	buf = ft_itoa_base(eval_expr(token->value->str + *i + 3), 10);
	if (g_arith_toomuch || g_arith_status == ERR)
	{
		free(buf);
		return (1);
	}
	token_replace_between(token, *i, end + 1, buf);
	*i += ft_strlen(buf) - 1;
	free(buf);
	return (0);
}
