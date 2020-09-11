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

void	increase_shlvl(void)
{
	char	*shlvl_str;
	char	buf[10];
	int		shlvl_int;

	shlvl_str = get_var_value("SHLVL");
	if (shlvl_str != NULL)
		shlvl_int = ft_atoi(shlvl_str) + 1;
	else
		shlvl_int = 1;
	if (shlvl_int >= 1000)
	{
		ft_dprintf(2, "42sh: warning: shell level (%d) too high, resetting to 1"
				, shlvl_int);
		shlvl_int = 1;
	}
	set_var("SHLVL", ft_itoa(shlvl_int, buf), V_EXPORT);
}

bool	file_may_be_binary(char *filename)
{
	int		fd;
	char	buf[80];
	int		ret;
	int		i;

	i = 0;
	if ((fd = open(filename, O_RDONLY)) >= 0)
		ret = read(fd, buf, 80);
	close(fd);
	if (fd < 0 || ret < 0)
	{
		ft_dprintf(STDERR_FILENO, "42sh: %s: unable to read file\n", filename);
		return (true);
	}
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
