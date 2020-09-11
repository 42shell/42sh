/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_op5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/11 19:48:54 by fratajcz          #+#    #+#             */
/*   Updated: 2020/09/11 19:48:54 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		test_gt(char **argv)
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
	if (ft_atoi(argv[1]) > ft_atoi(argv[3]))
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int		test_ge(char **argv)
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
	if (ft_atoi(argv[1]) >= ft_atoi(argv[3]))
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int		test_lt(char **argv)
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
	if (ft_atoi(argv[1]) < ft_atoi(argv[3]))
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int		test_le(char **argv)
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
	if (ft_atoi(argv[1]) <= ft_atoi(argv[3]))
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}
