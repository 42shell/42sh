/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_opt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 11:56:52 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/29 02:08:04 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static char	reset_get_opt(int *av_i, int *j, int *max_opt_argv)
{
	*av_i = 1;
	*j = 1;
	*max_opt_argv = -1;
	return (-1);
}

int			get_opt(int argc, unsigned char *argv[])
{
	static int	av_i = 1;
	static int	j = 1;
	static int	max_opt_argv = -1;

	if (max_opt_argv == -1)
	{
		max_opt_argv = 1;
		while (max_opt_argv < argc && argv[max_opt_argv][0] == '-'
				&& argv[max_opt_argv][1] != '\0')
			max_opt_argv++;
	}
	if (av_i == max_opt_argv || ft_strcmp(argv[av_i], "--") == 0)
		return (reset_get_opt(&av_i, &j, &max_opt_argv));
	if (argv[av_i][0] != '-' || (argv[av_i][j] == '\0' && j != 1))
	{
		av_i++;
		j = 1;
		return (get_opt(argc, argv));
	}
	return ((int)argv[av_i][j++]);
}
