/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_splitting.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 03:57:28 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/29 03:57:28 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static bool	is_ifs(char c)
{
	char	*ifs;

	if ((ifs = get_var_value("IFS")) == NULL)
		return (c == ' ' || c == '\t' || c == '\n');
	while (*ifs)
	{
		if (c == *ifs)
			return (true);
		ifs++;
	}
	return (false);
}

static int	get_wlen(t_token *token, int start, char *quote_status)
{
	char	*str;
	int		i;

	i = -1;
	str = token->value->str + start;
	while (str[++i]
		&& (!is_ifs(str[i]) || !was_expanded(token, i)
			|| *quote_status != NONE))
	{
		if (was_expanded(token, i))
			continue;
		if (quote_start(str, i, quote_status))
			continue;
		quote_stop(str, i, quote_status);
	}
	return (i);
}

static void	end_field_split(t_token *orig_token, t_token *new_list,
							t_token **list_last)
{
	if (new_list == NULL)
		return ;
	(*list_last) = orig_token->next;
	ft_dstr_del(&orig_token->value);
	ft_dstr_del(&orig_token->exp_info);
	*orig_token = *new_list;
	free(new_list);
}

void		split_fields(t_token *token)
{
	size_t	i;
	char	quote_status;
	t_token	**cur;
	t_token	*new_list;
	int		wlen;

	i = -1;
	quote_status = NONE;
	new_list = NULL;
	cur = &new_list;
	while (++i < token->value->len)
	{
		while (is_ifs(token->value->str[i]))
			i++;
		wlen = get_wlen(token, i, &quote_status);
		*cur = ft_xmalloc(sizeof(t_token));
		(*cur)->value = ft_dstr_from_strn(token->value->str + i, wlen);
		(*cur)->exp_info = ft_dstr_from_strn(token->exp_info->str + i, wlen);
		cur = &(*cur)->next;
		i += wlen;
	}
	end_field_split(token, new_list, cur);
}
