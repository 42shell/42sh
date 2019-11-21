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

#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <termios.h>
#include <term.h>
#include <curses.h>
#include <limits.h>
#include "input.h"

int		g_last_exit_st = 0;

int		main(void)
{
	char			c;
	struct s_term	term;

	init_term(&term);
	while (1)
	{
		read(STDIN_FILENO, &c, 1);
		write(1, &c, 1);
	}
	return (0);
}
