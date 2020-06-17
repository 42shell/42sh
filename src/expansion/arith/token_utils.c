/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/06 18:48:32 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/06 18:48:47 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arith.h"

bool	is_rel_op(enum e_token_type type)
{
	return (type >= LESS_EQUAL && type <= NOT_EQUAL);
}

bool	is_assignment(enum e_token_type type)
{
	return (type >= ASSIGN && type <= DIV_ASSIGN);
}
