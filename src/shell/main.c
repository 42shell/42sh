/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 19:37:33 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/10 17:39:45 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** If you want to check the leaks, you can uncomment the token_del() line,
** but don't put redirections operators in the input in this case,
** cause I use the previous token to set IO_NUMBER type, so if its deleted, it segfaults.
*/
int		main(int argc, char **argv, char **envp)
{
	struct s_sh	shell;

	(void)argc;
	(void)argv;
	(void)envp;
	init(&shell);
	while (1)
	{
		//shell.lexer.docdelim = ft_strdup("EOF");
        //while (eat(&shell.lexer) && *shell.lexer.curr_tok->value->str != ';')
		//	printf("[%s] -> ", shell.lexer.curr_tok->value->str);
		//printf("[%s] -> ", shell.lexer.curr_tok->value->str);
		//eat(&shell.lexer);
		//printf("[%s] -> ", shell.lexer.curr_tok->value->str);
		//shell.lexer.docdelim = ft_strdup("EOF2");
		//while (eat(&shell.lexer) != END_OF_INPUT)
		//	printf("[%s] -> ", shell.lexer.curr_tok->value->str);
		//printf("END\n");
		parse(&shell.lexer, &shell.env, &shell.term);
	}
	return (0);
}
