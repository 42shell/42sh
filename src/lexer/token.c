/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 21:45:49 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/29 00:02:48 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	token_del(t_token **tok)
{
	if (!tok || !*tok)
		return ;
	ft_dstr_del((void **)&(*tok)->value);
	ft_memdel((void **)tok);
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
