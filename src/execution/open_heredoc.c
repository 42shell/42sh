/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 14:38:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/28 20:23:29 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char	*g_tmp_file = NULL;

int		open_heredoc(t_dstr *heredoc)
{
	int		fd;

	free(g_tmp_file);
	g_tmp_file = ft_mktemp(ft_strdup("/tmp/42sh_XXXXXX"));
	while (g_tmp_file != NULL && (fd = open(g_tmp_file, O_WRONLY)) == -1)
	{
		free(g_tmp_file);
		g_tmp_file = ft_mktemp(ft_strdup("/tmp/42sh_XXXXXX"));
	}
	if (g_tmp_file == NULL)
		return (-1);
	param_expand(heredoc, 0, true);
	remove_bslash(heredoc);
	ft_putstr_fd(heredoc->str, fd);
	close(fd);
	fd = open(g_tmp_file, O_RDONLY);
	return (fd);
}
