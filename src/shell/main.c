/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 19:37:33 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/06 19:25:55 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_command		*get_complete_command(void)
{
	t_command	*complete_command;

	complete_command = parse_complete_command();
	if (g_parser.status != NOERR)
	{
		parse_error(g_parser.token ? g_parser.token->value->str : "EOF");
		complete_command_del(&complete_command);
		while (g_parser.token != NULL)
		{
			token_del(&g_parser.token);
			g_parser.token = get_next_token();
		}
		g_parser.heredocs = NULL;
		g_linebreak_type = 0;
		return (NULL);
	}
	if (complete_command && get_all_heredocs() != NOERR)
		complete_command_del(&complete_command);
	return (complete_command);
}

int				main_loop(void)
{
	t_command	*complete_command;

	while (1)
	{
		if (g_shell.jobs)
			notif_jobs();
		g_parser.status = NOERR;
		g_shell.get_input(PS1, false);
		if ((g_parser.token = get_next_token())
		&& (complete_command = get_complete_command()))
			eval_complete_command(complete_command);
		if (g_shell.interactive_mode
		&& g_lexer.line && *g_lexer.line != '\n')
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
