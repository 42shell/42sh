/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/02 00:15:06 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/29 16:31:25 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

/*
** reads fd into a local buffer and writes one char at a time into the given ptr
** same return values as read
*/

ssize_t	readc(int fd, void *ptr)
{
	static char		buf[4];
	static size_t	i;
	static size_t	chars_in_buf;
	ssize_t			ret;

	if (i == chars_in_buf || chars_in_buf == 0)
	{
		ret = read(fd, buf, sizeof(buf));
		i = 0;
		if (ret <= 0)
		{
			chars_in_buf = 0;
			return (ret);
		}
		chars_in_buf = ret;
	}
	*(unsigned char *)ptr = buf[i++];
	if (*(unsigned char *)ptr == '\0')
		return (readc(fd, ptr));
	return (1);
}
