/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mktemp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 18:17:19 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/29 18:19:40 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

#define LETTERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

char	*mktemp_return(char *template, int fd)
{
	if (fd >= 0)
		close(fd);
	else
	{
		free(template);
		return (NULL);
	}
	return (template);
}

/*
** template should be of the form {anything}XXXXXX
*/

char	*ft_mktemp(char *template)
{
	char				*xxxxxx;
	static const char	letters[] = LETTERS;
	static uint64_t		v;
	int					fd;

	fd = -1;
	xxxxxx = template + ft_strlen(template) - 6;
	v = INT64_MAX;
	while (v != 0)
	{
		xxxxxx[0] = letters[v % 62];
		xxxxxx[1] = letters[v % 61];
		xxxxxx[2] = letters[v % 60];
		xxxxxx[3] = letters[v % 59];
		xxxxxx[4] = letters[v % 58];
		xxxxxx[5] = letters[v % 57];
		v--;
		if ((fd = open(template, O_CREAT | O_EXCL, 420)) >= 0)
			break ;
	}
	return (mktemp_return(template, fd));
}
