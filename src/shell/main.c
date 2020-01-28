/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 19:37:33 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/23 10:55:57 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			main(int argc, char **argv)
{
	struct s_sh	shell;

	init(&shell, --argc, ++argv);
	printf("%.1lf\n", 152.29);
	ft_printf("%.1lf\n", 152.29);
	while (1)
		parse(&shell.lexer, &shell.env, &shell.term);
	return (0);
}
