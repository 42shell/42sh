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

static void	remove_dquotes(t_token *token, int start)
{
	int i;

	i = start;
	while (token->value->str[i])
	{
		if (token->value->str[i] == '\\' && token->value->str[i + 1] == '\"')
		{
			if (token->exp_info)
				ft_dstr_remove(token->exp_info, i, 1);
			ft_dstr_remove(token->value, i++, 1);
		}
		else if (token->value->str[i] == '\"')
		{
			if (token->exp_info)
				ft_dstr_remove(token->exp_info, i, 1);
			ft_dstr_remove(token->value, i, 1);
		}
		else
			i++;
	}
}

static void	replace_by_buf(t_token *token, int *i, char *buf)
{
	size_t	to_replace_len;
	size_t	buf_len;

	to_replace_len = ft_strlen(token->value->str + *i) + 2;
	buf_len = ft_strlen(buf);
	ft_dstr_remove(token->value, *i, to_replace_len);
	ft_dstr_insert(token->value, *i, buf, buf_len);
	if (token->exp_info)
	{
		ft_dstr_remove(token->exp_info, *i, to_replace_len);
		ft_dstr_insert(token->exp_info, *i, buf, buf_len);
		ft_memset(token->exp_info->str + *i, '1', buf_len);
	}
	*i += buf_len - 1;
	free(buf);
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

	end = get_end_of_braces(token->value->str + *i) - 1;
	token->value->str[end + *i] = '\0';
	token->exp_info->str[end + *i] = '\0';
	remove_dquotes(token, *i + 3);
	if (dollar_expand(token, *i + 3, false) == 1)
		return (1);
	buf = ft_itoa_base(eval_expr(token->value->str + *i + 3), 10);
	if (g_arith_toomuch || g_arith_status == ERR)
	{
		free(buf);
		return (1);
	}
	replace_by_buf(token, i, buf);
	return (0);
}
