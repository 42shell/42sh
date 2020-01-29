/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:09:52 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/29 15:13:53 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

bool	is_operator_start(char c)
{
	return (c == '<' || c == '>' || c == '&' || c == ';' || c == '|');
}

bool	is_operator_part(char c)
{
	return ((is_operator_start(c)) || c == '-');
}

bool	is_redir(t_token *token)
{
	return (LESS <= token->type && token->type <= GREATAND);
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
	if (ft_strequ(ope, "<"))
		return (LESS);
	if (ft_strequ(ope, ">"))
		return (GREAT);
	if (ft_strequ(ope, "<<"))
		return (DLESS);
	if (ft_strequ(ope, ">>"))
		return (DGREAT);
	if (ft_strequ(ope, "<&"))
		return (LESSAND);
	if (ft_strequ(ope, ">&"))
		return (GREATAND);
	if (ft_strequ(ope, "&"))
		return (AMPERSAND);
	if (ft_strequ(ope, "&&"))
		return (AND_IF);
	if (ft_strequ(ope, "|"))
		return (PIPE);
	if (ft_strequ(ope, "||"))
		return (OR_IF);
	if (ft_strequ(ope, ";"))
		return (SEMI);
	return (0);
}
