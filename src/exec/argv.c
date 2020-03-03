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

char		**get_argv(t_node *command_node)
{
	char	**argv;
	int		argc;
	int		i;

	i = 0;
	argc = 0;
	if (!command_node->argv)
		return (NULL);
	while (command_node->argv[argc])
		argc++;
	array = ft_xmalloc(sizeof(char *) * (argc + 1));
	while (i < argc)
	{
		argv[i] = ft_strdup(command_node->argv[i]->value->str);
		i++;
	}
	return (argv);
}
