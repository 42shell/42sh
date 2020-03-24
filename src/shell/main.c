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

t_list			*get_list(void)
{
	t_list		*list;

	if ((list = ps_list())
	&& ps_get_all_heredocs() == NOERR)
		return (list);
	else if (g_parser.error)
	{
		ps_error(g_parser.token ? g_parser.token->value->str : "(null)");
		token_del(&g_parser.token);
	}
	return (NULL);
}

int				main_loop(void)
{
	t_list		*list;
	t_job		*job;

	while (1) //while !g_shell.quit...
	{
		g_parser.error = NOERR;
		g_shell.get_input(PS1);
		if ((g_parser.token = get_next_token())
		&& (list = get_list()))
		{
			while (list)
			{
				//print_ast(list->ast, 0);
				job = job_new(list->ast, 0, 1);
				add_job(job);
				launch_job(job);
				list = list->next;
			}
			list_del(&list); //not del asts
			if (g_shell.interactive_mode && g_lexer.line)
			{
				g_lexer.line[ft_strlen(g_lexer.line) - 1] = 0;
				rl_add_history(g_lexer.line);
			}
		}
		reset_lexer();
	}
	return (0);
}

int				main(int argc, char **argv)
{
	init(argc - 1, argv + 1);
	main_loop();
	return (0);
}
