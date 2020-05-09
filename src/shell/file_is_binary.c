/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_is_binary.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/08 18:38:11 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 18:38:12 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

bool	file_is_binary(char *filename)
{
	int		fd;
	char	buf[80];
	int		i;

	i = 0;
	if ((fd = open(filename, O_RDONLY)) == -1 || read(fd, buf, 80) == -1)
	{
		ft_dprintf(STDERR_FILENO, "42sh: %s: unable to read file\n");
		close(fd);
		return (true);
	}
	while (i < 80 && buf[i] && buf[i] != '\n')
		i++;
	if (i < 80 && buf[i] == 0)
	{
		ft_dprintf(STDERR_FILENO,
		"42sh: %s: cannot execute binary file\n", filename);
		close(fd);
		return (true);
	}
	close(fd);
	return (false);
}
