/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 19:18:20 by fratajcz          #+#    #+#             */
/*   Updated: 2020/11/09 19:18:20 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char	*find_dot(const char *path)
{
	int	i;

	if (path[0] == '.' && path[1] == '/')
		return ((char *)path);
	i = 0;
	while (path[i] != '\0')
	{
		if (path[i] == '/' && path[i + 1] == '.'
				&& (path[i + 2] == '/' || path[i + 2] == '\0'))
			return ((char *)(path + i));
		i++;
	}
	return (NULL);
}

char	*find_dotdot(const char *path)
{
	int	i;

	i = 0;
	while (path[i] != '\0')
	{
		if (path[i] == '/' && path[i + 1] == '.' && path[i + 2] == '.'
				&& (path[i + 3] == '/' || path[i + 3] == '\0'))
			return ((char *)(path + i));
		i++;
	}
	return (NULL);
}
