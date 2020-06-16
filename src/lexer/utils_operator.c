/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_operator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:09:52 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 18:31:35 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

bool	is_operator_start(char c)
{
	return (c == '<' || c == '>' || c == '&' || c == ';' || c == '|'
	|| c == '(' || c == ')');
}

bool	is_operator_part(char c)
{
	return ((is_operator_start(c)) || c == '-');
}

bool	is_operator_next(char *ope, char c)
{
	if (is_operator_part(c))
	{
		if (c == '<')
			return (ft_strequ(ope, "<"));
		else if (c == '>')
			return (ft_strequ(ope, ">"));
		else if (c == '&')
			return (ft_strequ(ope, "<")
			|| ft_strequ(ope, ">")
			|| ft_strequ(ope, "&"));
		else if (c == '|')
			return (ft_strequ(ope, "|"));
	}
	return (0);
}

int		get_operator_type(char *ope)
{
	if (ft_strequ(ope, "<") || ft_strequ(ope, ">"))
		return (*ope == '<' ? LESS : GREAT);
	if (ft_strequ(ope, "<<") || ft_strequ(ope, ">>"))
		return (*ope == '<' ? DLESS : DGREAT);
	if (ft_strequ(ope, "<&") || ft_strequ(ope, ">&"))
		return (*ope == '<' ? LESSAND : GREATAND);
	if (ft_strequ(ope, "&") || ft_strequ(ope, "&&"))
		return (*(ope + 1) == 0 ? AMPERSAND : AND_IF);
	if (ft_strequ(ope, "|") || ft_strequ(ope, "||"))
		return (*(ope + 1) == 0 ? PIPE : OR_IF);
	if (ft_strequ(ope, "(") || ft_strequ(ope, ")"))
		return (*ope == '(' ? LBRACKET : RBRACKET);
	if (ft_strequ(ope, ";"))
		return (SEMI);
	return (0);
}
