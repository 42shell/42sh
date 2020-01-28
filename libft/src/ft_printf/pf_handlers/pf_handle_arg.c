/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_handle_arg.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 12:32:10 by nbousset          #+#    #+#             */
/*   Updated: 2019/05/04 17:01:01 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	handle_strings(t_printf *pf)
{
	return (pf_print_string(pf));
}

static void	handle_int(t_printf *pf)
{
	if (pf->conv == 'd' || pf->conv == 'D' || pf->conv == 'i')
		return (pf_print_int(pf));
	else if (pf->conv == 'o' || pf->conv == 'O')
		return (pf_print_uint(pf, 8));
	else if (pf->conv == 'u' || pf->conv == 'U')
		return (pf_print_uint(pf, 10));
	else if (pf->conv == 'x' || pf->conv == 'X'
			|| pf->conv == 'p' || pf->conv == 'P')
		return (pf_print_uint(pf, 16));
	else if (pf->conv == 'b' || pf->conv == 'B')
		return (pf_print_uint(pf, 2));
}

void		pf_handle_arg(t_printf *pf)
{
	if (ft_strchr("dDioOuUxXbBpP", pf->conv))
		return (handle_int(pf));
	else if (ft_strchr("cCsS%", pf->conv))
		return (handle_strings(pf));
}
