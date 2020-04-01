/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 19:37:33 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/01 16:38:37 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_node			*get_ast(void)
{
	t_node		*ast;

	if ((ast = parse_list())
	&& parse_get_all_heredocs() == NOERR)
		return (ast);
	else if (g_parser.error)
	{
		parse_error(g_parser.token ? g_parser.token->value->str : "(null)");
		token_del(&g_parser.token);
		g_parser.error = NOERR;
	}
	return (NULL);
}

int				main_loop(void)
{
	t_node		*ast;

	while (1)
	{
		if (g_shell.jobs)
			notif_jobs();
		g_shell.get_input(PS1);
		if ((g_parser.token = get_next_token())
		&& (ast = get_ast()))
		{
			//print_ast(ast, 0);
			eval_ast(ast);
			ast_del(&ast);
		}
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
