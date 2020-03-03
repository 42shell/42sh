/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/14 17:52:18 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char		**get_argv(t_command *command)
{
	char	**argv;
	int		argc;
	int		i;

	i = 0;
	argc = 0;
	if (!command->argv)
		return (NULL);
	while (command->argv[argc])
		argc++;
	argv = ft_xmalloc(sizeof(char *) * (argc + 1));
	while (i < argc)
	{
		argv[i] = ft_strdup(command->argv[i]->value->str);
		i++;
	}
	return (argv);
}
