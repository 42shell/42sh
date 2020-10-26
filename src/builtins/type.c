/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 14:40:25 by fratajcz          #+#    #+#             */
/*   Updated: 2020/09/20 14:40:25 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		builtin_type(char **argv, t_array *env)
{
	int		ret;
	int		i;
	char	*path;

	ret = 0;
	i = 1;
	while (argv[i])
	{
		if (is_builtin(argv[i]))
			ft_printf("%s is a shell builtin\n", argv[i]);
		else if (is_reserved_word(argv[i]))
			ft_printf("%s is a shell keyword\n", argv[i]);
		else if ((path = get_exec_path(argv[i], env)))
		{
			ft_printf("%s is %s\n", argv[i], path);
			free(path);
		}
		else
		{
			ft_dprintf(2, "42sh: type: %s: not found\n", argv[i]);
			ret = 1;
		}
		i++;
	}
	return (ret);
}
