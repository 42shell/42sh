/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 19:37:33 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/09 23:30:03 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

extern int g_exec_status;

t_command		*get_command_list(void)
{
	t_command *command_list;

	if ((command_list = parse_command_list())
	&& parse_get_all_heredocs() == NOERR)
		return (command_list);
	parse_newline_list();
	if (g_parser.status || g_parser.token != NULL)
	{
		parse_error(g_parser.token ? g_parser.token->value->str : "(null)");
		token_del(&g_parser.token);
		g_parser.status = NOERR;
	}
	return (NULL);
}

int				main_loop(void)
{
	t_command *command_list;

	while (1)
	{
		if (g_shell.jobs)
			notif_jobs();
		g_shell.get_input(PS1);
		if ((g_parser.token = get_next_token())
		&& (command_list = get_command_list()))
		{
			g_exec_status = 0;
			eval_command_list(command_list);
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
