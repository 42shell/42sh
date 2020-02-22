/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term_display.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:08:08 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/16 15:39:06 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readline.h"

/*
** this was used for scroll, I keep it for later use
** int		tputnstr(char *str, size_t n)
** {
** 	size_t	len;
** 
** 	len = 0;
** 	while (*str && n--)
** 	{
** 		if (*str == '\n')
** 			movcnl();
** 		else if (write(STDOUT_FILENO, str, 1))
** 		{
** 			if (g_rl_posx == g_rl_sizex - 1 && n)
** 				movcnl();
** 			else
** 				g_rl_posx++;
** 		}
** 		len++;
** 		str++;
** 	}
** 	return (len);
** }
*/

int		tputstr(const char *str)
{
	size_t	len;

	len = 0;
	while (*str)
	{
		if (*str == '\n')
			movcnl();
		else if (write(STDOUT_FILENO, str, 1))
		{
			if (g_rl_posx == g_rl_sizex - 1)
				movcnl();
			else
				g_rl_posx++;
		}
		len++;
		str++;
	}
	return (len);
}
