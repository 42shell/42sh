/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 19:37:33 by fratajcz          #+#    #+#             */
/*   Updated: 2019/11/22 01:15:03 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		g_last_exit_st = 0;

int		main(void)
{
	struct s_sh		shell;

	int i = 1;
	init_shell(&shell);
	while (i)
	{
		read_line(&shell.term, &shell.input);
		i = 0;
		//parse
		//exec
	}
	return (0);
}
