/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 04:37:16 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 18:35:05 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** pipeline     : command
**              | command pipe_list
**
** pipe_list    : '|' linebreak command pipe_list
**              | EMPTY
**
**
**                 |
**               /   \
**              |    test
**           /     \
**         ls      cat
*/

t_command	*parse_pipe_list(t_command *left_operand)
{
	t_command	*pipeline;
	t_command	*right_operand;
	int			type;

	if (g_parser.token == NULL || (type = g_parser.token->type) != PIPE
			|| left_operand == NULL)
		return (left_operand);
	token_del(&g_parser.token);
	g_parser.token = get_next_token();
	parse_linebreak(type);
	right_operand = parse_command();
	if (right_operand == NULL)
	{
		if (g_parser.status == NOERR)
			g_parser.status = UNEXPECTED_TOKEN;
		command_del(&left_operand);
		return (NULL);
	}
	pipeline = command_new(CONNECTION);
	pipeline->value.connection->connector = type;
	pipeline->value.connection->left = left_operand;
	pipeline->value.connection->right = right_operand;
	return (parse_pipe_list(pipeline));
}

t_command	*parse_pipeline(void)
{
	return (parse_pipe_list(parse_command()));
}
