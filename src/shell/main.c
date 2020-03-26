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

t_node			*get_ast(void)
{
	t_node		*ast;

	if ((ast = ps_list())
	&& ps_get_all_heredocs() == NOERR)
		return (ast);
	else if (g_parser.error)
	{
		ps_error(g_parser.token ? g_parser.token->value->str : "(null)");
		token_del(&g_parser.token);
	}
	return (NULL);
}

int				main_loop(void)
{
	t_node		*ast;
	t_job		*job;

	while (1) //while !g_shell.quit...
	{
		g_parser.error = NOERR;
		if (g_shell.jobs)
			notif_jobs();
		g_shell.get_input(PS1);//remove
		if ((g_parser.token = get_next_token())
		&& (ast = get_ast()))
		{
			//print_ast(ast, 0);
			//eval_ast(ast);
			job = job_new(ast, STDIN_FILENO, STDOUT_FILENO);
			add_job(job);
			launch_job(job);
		}
		//ast_del(&ast); //not del asts
		if (g_shell.interactive_mode && g_lexer.line)
		{
			g_lexer.line[ft_strlen(g_lexer.line) - 1] = 0;
			rl_add_history(g_lexer.line);
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
