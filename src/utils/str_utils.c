/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:58:50 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/29 18:19:43 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char	*ft_strjoin_triple(const char *s1, const char *s2, const char *s3)
{
	char *str;

	if (!(str = ft_strnew(ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3))))
		return (NULL);
	ft_strcpy(str, s1);
	ft_strcat(str, s2);
	return (ft_strcat(str, s3));
}

bool	was_expanded(t_token *token, int i)
{
	if (token->exp_info)
		return (token->exp_info->str[i] == '1');
	else
		return (false);
}

/*
** returns true if we have a subshell paren inside $() or <()
** not if it's an actual subshell like (ls)
*/

bool	is_subshell_paren(const char *str, int i, enum e_quote_st brack_status)
{
	if (str[i] != '(')
		return (false);
	if (i == 0)
		return (brack_status == PAREN);
	return ((str[i - 1] != '$' && str[i - 1] != '<' && str[i - 1] != '>')
		&& brack_status == PAREN);
}

bool	is_procsub_paren(const char *str, int i)
{
	return ((str[i] == '<' || str[i] == '>') && str[i + 1] == '(');
}
