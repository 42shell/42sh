/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 19:37:33 by fratajcz          #+#    #+#             */
/*   Updated: 2019/11/21 20:39:42 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/shell.h"
//#include "../include/term.h"

int		g_last_exit_st = 0;

int		main(void)
{
	char			c;
	struct s_sh		shell;

	init_shell(&shell);
	while (1)
	{
		read(STDIN_FILENO, &c, 1);
		write(1, &c, 1);
	}
	return (0);
}
