/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_op1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/11 19:43:43 by fratajcz          #+#    #+#             */
/*   Updated: 2020/09/11 19:43:43 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		test_b(char **argv)
{
	struct stat b;

	if (stat(argv[2], &b) != 0)
		return (EXIT_FAILURE);
	if (S_ISBLK(b.st_mode))
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int		test_c(char **argv)
{
	struct stat b;

	if (stat(argv[2], &b) != 0)
		return (EXIT_FAILURE);
	if (S_ISCHR(b.st_mode))
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int		test_d(char **argv)
{
	struct stat b;

	if (stat(argv[2], &b) != 0)
		return (EXIT_FAILURE);
	if (S_ISDIR(b.st_mode))
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int		test_e(char **argv)
{
	struct stat b;

	if (stat(argv[2], &b) == 0)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int		test_f(char **argv)
{
	struct stat b;

	if (stat(argv[2], &b) != 0)
		return (EXIT_FAILURE);
	if (S_ISREG(b.st_mode))
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}
