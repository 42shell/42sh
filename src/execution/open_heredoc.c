/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 14:38:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/22 18:33:39 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		open_heredoc(t_dstr *heredoc)
{
	char	*tmp_file;
	int		fd;

	tmp_file = ft_mktemp(ft_strdup("/tmp/42sh_XXXXXX"));
	while (tmp_file != NULL && (fd = open(tmp_file, O_WRONLY)) == -1)
	{
		free(tmp_file);
		tmp_file = ft_mktemp(ft_strdup("/tmp/42sh_XXXXXX"));
	}
	if (tmp_file == NULL)
		return (-1);
	param_expand(heredoc, 0, g_env, true);
	remove_bslash(heredoc);
	ft_putstr_fd(heredoc->str, fd);
	close(fd);
	fd = open(tmp_file, O_RDONLY);
	free(tmp_file);
	return (fd);
}
