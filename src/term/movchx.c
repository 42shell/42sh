/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movchx.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:08:08 by fratajcz          #+#    #+#             */
/*   Updated: 2019/11/22 00:46:27 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	movchx(t_term *term)
{
	tputs(term->caps[C_CR], 1, ft_putc);
	term->cx = 0;
}
