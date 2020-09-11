/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_op2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/11 19:47:28 by fratajcz          #+#    #+#             */
/*   Updated: 2020/09/11 19:47:28 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		test_g(char **argv)
{
	struct stat b;

	if (stat(argv[2], &b) != 0)
		return (EXIT_FAILURE);
	if (S_ISGID & b.st_mode)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int		test_l(char **argv)
{
	struct stat b;

	if (lstat(argv[2], &b) != 0)
		return (EXIT_FAILURE);
	if (S_ISLNK(b.st_mode))
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int		test_p(char **argv)
{
	struct stat b;

	if (stat(argv[2], &b) != 0)
		return (EXIT_FAILURE);
	if (S_ISFIFO(b.st_mode))
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int		test_r(char **argv)
{
	struct stat b;

	if (stat(argv[2], &b) != 0)
		return (EXIT_FAILURE);
	if (access(argv[2], R_OK) == 0)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int		test_sock(char **argv)
{
	struct stat b;

	if (stat(argv[2], &b) != 0)
		return (EXIT_FAILURE);
	if (S_ISSOCK(b.st_mode))
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}
