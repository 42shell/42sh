/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/10 12:58:31 by fratajcz          #+#    #+#             */
/*   Updated: 2019/04/10 20:10:02 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <stdlib.h>
#include "libft.h"
#include <limits.h>

#define BUFF_SIZE 256

int		process_ln(char **line, char **buffer, char *pos_nl)
{
	char	*tmp;
	int		line_len;

	line_len = pos_nl - *buffer;
	if ((*line = malloc(line_len + 1)) == NULL)
		return (-1);
	ft_strncpy(*line, *buffer, line_len);
	(*line)[line_len] = '\0';
	if ((tmp = ft_strsub(*buffer, line_len + 1, ft_strlen(pos_nl))) == NULL)
		return (-1);
	free(*buffer);
	*buffer = tmp;
	return (1);
}

int		get_next_line(const int fd, char **line)
{
	static char		*buffer;
	char			buf[BUFF_SIZE + 1];
	char			*tmp;
	int				nb_read;
	char			*pos_nl;

	if (fd < 0)
		return (-1);
	if (!(buffer = (buffer == NULL) ? ft_strnew(1) : buffer))
		return (-1);
	if ((pos_nl = ft_strchr(buffer, '\n')) != NULL)
		return (process_ln(line, &buffer, pos_nl));
	while ((nb_read = read(fd, buf, BUFF_SIZE)) > 0)
	{
		buf[nb_read] = '\0';
		if (!(tmp = ft_strjoin(buffer, buf)))
			return (-1);
		free(buffer);
		buffer = tmp;
		if ((pos_nl = ft_strchr(buffer, '\n')) != NULL)
			return (process_ln(line, &buffer, pos_nl));
	}
	if (*(buffer))
		return (process_ln(line, &buffer, ft_strchr(buffer, '\0')));
	return (nb_read < 0 ? -1 : 0);
}
