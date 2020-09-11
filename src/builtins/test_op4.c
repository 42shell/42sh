/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_op4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/11 19:48:24 by fratajcz          #+#    #+#             */
/*   Updated: 2020/09/11 19:48:24 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		test_z(char **argv)
{
	if (argv[2][0] == '\0')
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int		test_equal(char **argv)
{
	if (ft_strequ(argv[1], argv[3]))
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int		test_unequal(char **argv)
{
	return (!test_equal(argv));
}

int		test_eq(char **argv)
{
	if (!is_number(argv[1]))
	{
		ft_dprintf(2, "42sh: test: %s: integer expression expected\n", argv[1]);
		return (2);
	}
	else if (!is_number(argv[3]))
	{
		ft_dprintf(2, "42sh: test: %s: integer expression expected\n", argv[3]);
		return (2);
	}
	if (ft_atoi(argv[1]) == ft_atoi(argv[3]))
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int		test_ne(char **argv)
{
	if (!is_number(argv[1]))
	{
		ft_dprintf(2, "42sh: test: %s: integer expression expected\n", argv[1]);
		return (2);
	}
	else if (!is_number(argv[3]))
	{
		ft_dprintf(2, "42sh: test: %s: integer expression expected\n", argv[3]);
		return (2);
	}
	if (ft_atoi(argv[1]) != ft_atoi(argv[3]))
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}
