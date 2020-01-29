/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:09:52 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/29 15:04:17 by fratajcz         ###   ########.fr       */
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
	return (LESS <= token->type && token->type <= DLESSDASH);
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
		else if (c == '-')
			return (ft_strequ(ope, "<<"));
	}
	return (0);
}

int		get_operator_type(char *ope)
{
	int		ret;

	if ((ft_strequ(ope, "<") && (ret = LESS))
	|| (ft_strequ(ope, ">") && (ret = GREAT))
	|| (ft_strequ(ope, "<<") && (ret = DLESS))
	|| (ft_strequ(ope, ">>") && (ret = DGREAT))
	|| (ft_strequ(ope, "<&") && (ret = LESSAND))
	|| (ft_strequ(ope, ">&") && (ret = GREATAND))
	|| (ft_strequ(ope, "<<-") && (ret = DLESSDASH))
	|| (ft_strequ(ope, "&") && (ret = AMPERSAND))
	|| (ft_strequ(ope, "&&") && (ret = AND_IF))
	|| (ft_strequ(ope, "|") && (ret = PIPE))
	|| (ft_strequ(ope, "||") && (ret = OR_IF))
	|| (ft_strequ(ope, ";") && (ret = SEMI)))
		return (ret);
	return (0);
}
