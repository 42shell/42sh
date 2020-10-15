/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 21:45:49 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/26 02:27:49 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_token	*token_list_dup(t_token *list)
{
	t_token	*dup;
	t_token	*tmp;
	t_token	*last;

	dup = NULL;
	if (!list)
		return (NULL);
	while (list)
	{
		tmp = token_dup(list);
		if (!dup)
		{
			dup = tmp;
			last = dup;
		}
		else
		{
			last->next = tmp;
			last = last->next;
		}
		list = list->next;
	}
	return (dup);
}

t_token	*token_dup(t_token *token)
{
	t_token	*dup;

	if (!token)
		return (NULL);
	dup = token_new(token->type);
	ft_dstr_append(dup->value, token->value->str);
	return (dup);
}

void	token_list_del(t_token **token)
{
	if (!token || !*token)
		return ;
	if ((*token)->next)
		token_list_del(&(*token)->next);
	ft_dstr_del(&(*token)->value);
	if ((*token)->exp_info)
		ft_dstr_del(&(*token)->exp_info);
	ft_memdel((void **)token);
}

void	token_del(t_token **token)
{
	if (!token || !*token)
		return ;
	ft_dstr_del(&(*token)->value);
	if ((*token)->exp_info)
		ft_dstr_del(&(*token)->exp_info);
	ft_memdel((void **)token);
}

t_token	*token_new(int type)
{
	t_token	*token;

	if (!(token = (t_token *)ft_xmalloc(sizeof(*token))))
		return (NULL);
	token->value = ft_dstr_new(16);
	token->type = type;
	return (token);
}
