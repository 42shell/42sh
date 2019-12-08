/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_new.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:09:52 by fratajcz          #+#    #+#             */
/*   Updated: 2019/11/27 14:56:00 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_token *token_new(int type)
{
    t_token *token;

    if (!(token = (t_token *)ft_memalloc(sizeof(*token))))
        return (NULL);
    if (!(token->content = ft_dstr_new(1)))
    {
        ft_memdel((void *)&token);
        return (NULL);
    }
    token->is_delim = 0;
    token->type = type;
    return (token);
}