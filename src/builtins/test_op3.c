/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_op3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/11 19:47:57 by fratajcz          #+#    #+#             */
/*   Updated: 2020/09/11 19:47:57 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		test_size(char **argv)
{
	struct stat b;

	if (stat(argv[2], &b) != 0)
		return (EXIT_FAILURE);
	if (b.st_size > 0)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int		test_t(char **argv)
{
	if (!is_number(argv[2]))
		return (EXIT_FAILURE);
	if (isatty(ft_atoi(argv[2])))
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int		test_u(char **argv)
{
	struct stat b;

	if (stat(argv[2], &b) != 0)
		return (EXIT_FAILURE);
	if (S_ISUID & b.st_mode)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int		test_w(char **argv)
{
	struct stat b;

	if (stat(argv[2], &b) != 0)
		return (EXIT_FAILURE);
	if (access(argv[2], W_OK) == 0)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int		test_x(char **argv)
{
	struct stat b;

	if (stat(argv[2], &b) != 0)
		return (EXIT_FAILURE);
	if (access(argv[2], X_OK) == 0)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}
