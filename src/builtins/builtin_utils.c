/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:58:50 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/29 18:19:43 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

bool	is_builtin(char *str)
{
	return (ft_strcmp(str, "echo") == 0 || ft_strcmp(str, "cd") == 0
		|| ft_strcmp(str, "setenv") == 0 || ft_strcmp(str, "unsetenv") == 0
		|| ft_strcmp(str, "env") == 0 || ft_strcmp(str, "exit") == 0);
}
