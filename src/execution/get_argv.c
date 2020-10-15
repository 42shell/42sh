/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_argv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/10 14:11:39 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char	**get_argv(t_simple_cmd *command)
{
	t_token	*arg;
	char	**argv;
	int		count;

	count = 0;
	arg = command->args_exp;
	if (!arg)
		return (NULL);
	while (arg)
	{
		count++;
		arg = arg->next;
	}
	argv = ft_xmalloc(sizeof(char *) * (count + 1));
	arg = command->args_exp;
	count = 0;
	while (arg)
	{
		argv[count++] = ft_strdup(arg->value->str);
		arg = arg->next;
	}
	return (argv);
}
