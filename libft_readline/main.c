/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 23:03:42 by fratajcz          #+#    #+#             */
/*   Updated: 2019/11/21 23:05:58 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "readline.h"
#include <stdio.h>

int		main(int argc, char **argv)
{
	char	*line;

	while ((line = readline("$> ")) && !ft_strequ(line, "exit")
	&& !(g_rl_last_ret_the_last_one_not_the_previous_one == 4))
	{
		printf("%s\n", line);
		free(line);
	}
	return (0);
}
