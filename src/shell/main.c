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

int			get_input(const char *prompt)
{
	char	*line;
	char	*tmp;
	//int		ret;

	if (g_shell.interactive_mode)
	{
		if (!(line = readline(prompt)))
			exit(1);//error()
		else if (!*line) //ctrl-C || ctrl-D
		{
			if (g_rl_last_ret == RL_EOF)
				exit(0);//builtin_exit()
			free(line);
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
	if (g_lexer.line)
	{
		tmp = g_lexer.line;
		g_lexer.line = ft_strjoin(g_lexer.line, line);
		free(tmp);
	}
	else
		g_lexer.line = ft_strdup(line);
	free(line);
	return (0);
}

int			main(int argc, char **argv)
{
	t_job	*jobs;
	t_job	*ptr;
	int		ret;

	init(argc - 1, argv + 1);
	while ((ret = get_input(PS1)) != RL_EOF)
	{
		g_parser.parse_error = NOERR;
		if ((jobs = get_jobs()) && !g_parser.parse_error)
		{
			ptr = jobs;
			while (ptr)
			{
				printf("SF\n");
				print_ast(ptr->ast, 0);//run(ast, env);
				printf("\n");
				ptr = ptr->next;
			}
			//run(ast);
		}
		g_lexer.line[ft_strlen(g_lexer.line) - 1] = 0;
		rl_add_history(g_lexer.line);
		reset_lexer();
		del_jobs(&jobs);
	}
	return (0);
}
