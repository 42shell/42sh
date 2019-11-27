/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del_shell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:08:08 by fratajcz          #+#    #+#             */
/*   Updated: 2019/11/22 00:37:04 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void	del_input(t_input *input)
{
	ft_strarray_del(&input->pmpt);
	line_del(&input->line);
}

static void	del_term(t_term *term)
{
	term->termtype = NULL;
	return ;
}

void		del_shell(t_sh *shell)
{
	del_term(&shell->term);
	del_input(&shell->input);
}
