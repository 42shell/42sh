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

int			main_loop()
{
	t_ast	*ast_ptr;

	while (1) //while !g_shell.quit...
	{
		g_parser.error = NOERR;
		g_shell.get_input(PS1);
		if ((g_shell.ast = complete_command()))
		{
			ast_ptr = g_shell.ast;
			while (ast_ptr && ast_ptr->root)
			{
				print_ast(ast_ptr->root, 0);
				//printf("\n");
				ast_ptr = ast_ptr->next;
			}
			//run(ast);
			ast_del(&g_shell.ast);
		}
		if (g_shell.interactive_mode && g_lexer.line)
		{
			g_lexer.line[ft_strlen(g_lexer.line) - 1] = 0;
			rl_add_history(g_lexer.line);
		}
		if (g_shell.interactive_mode)
			reset_lexer();
	}
	return (0);
}

int			main(int argc, char **argv)
{
	init(argc - 1, argv + 1);
	main_loop();
	return (0);
}
