/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:58:50 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/29 18:19:43 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

bool	is_valid_var_name(char *str)
{
	if (!ft_isalpha(*str))
		return (false);
	while (*str)
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (false);
		str++;
	}
	return (true);
}

char	*ft_strjoin_triple(char *s1, char *s2, char *s3)
{
	char *str;

	if (!(str = ft_strnew(ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3))))
		return (NULL);
	ft_strcpy(str, s1);
	ft_strcat(str, s2);
	return (ft_strcat(str, s3));
}

bool	is_builtin(char *str)
{
	return (ft_strcmp(str, "echo") == 0 || ft_strcmp(str, "cd") == 0
		|| ft_strcmp(str, "setenv") == 0 || ft_strcmp(str, "unsetenv") == 0
		|| ft_strcmp(str, "env") == 0 || ft_strcmp(str, "exit") == 0);
}

void	free_arr(char **arr)
{
	int i;

	if (arr == NULL)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}
