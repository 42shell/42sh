/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 19:37:33 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/23 10:55:57 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <stdio.h>

int			get_input(const char *prompt)
{
	char	*line;
	//int		ret;

	if (g_interactive_mode)
	{
		if (!(line = readline(prompt)))
			exit(1);//error()
		else if (!*line) //ctrl-C || ctrl-D
		{
			if (g_rl_last_ret == RL_EOF)
				exit(0);//builtin_exit()
			return (get_input(prompt));
		}
	}
	else
	{
		//if ((ret = get_next_line(0, line)) == -1);
		//	exit(1);//error();
		//else if (ret == 0)
		//	return (EOF);
		return (0);
	}
	g_line = g_line ? ft_strjoin(g_line, line) : ft_strdup(line);
	return (0);
}

int			main(int argc, char **argv)
{
	int		ret;
	t_token	*token;

	init(argc - 1, argv + 1);
	while ((ret = get_input(PS1)) != RL_EOF)
	{
		while ((token = get_next_token()))
			printf("[%d: %s] -> ", token->type, token->value->str);
		printf("END\n");
		/*
		parse(ast, g_line);
		execute(ast);
		*/
		rl_add_history(g_line);
		free(g_line);
		g_line = NULL;
	}
	return (0);
}
