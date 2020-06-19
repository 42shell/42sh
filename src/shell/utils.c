/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/29 15:52:10 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/29 15:52:12 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

bool	file_may_be_binary(char *filename)
{
	int		fd;
	char	buf[80];
	int		ret;
	int		i;

	i = 0;
	if (!filename)
		return (false);
	if ((fd = open(filename, O_RDONLY)) < 0 || (ret = read(fd, buf, 80)) < 0)
	{
		close(fd);
		ft_dprintf(STDERR_FILENO, "42sh: %s: unable to read file\n", filename);
		return (true);
	}
	close(fd);
	while (i < ret && buf[i] != '\n')
	{
		if (buf[i++] == '\0')
		{
			ft_dprintf(STDERR_FILENO,
			"42sh: %s: cannot execute binary file\n", filename);
			return (true);
		}
	}
	return (false);
}

void	normalize_lexer_line(void)
{
	size_t	len;

	len = ft_strlen(g_lexer.line);
	if (g_lexer.i > len)
		g_lexer.i = len;
}
