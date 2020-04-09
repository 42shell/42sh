/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and_or.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/09 23:57:07 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** and_or       : pipeline and_or_list
**
** and_or_list  : AND_IF linebreak pipeline and_or_list
**              | OR_IF linebreak pipeline and_or_list
**              | EMPTY
**
**
**                 &&
**               /   \
**             ||    test
**           /    \
**         ls     cat
*/

t_command	*parse_and_or_list(t_command *left_operand)
{
	t_command	*and_or;
	t_command	*right_operand;
	int			type;

	if (g_parser.token == NULL
			|| ((type = g_parser.token->type) != AND_IF && type != OR_IF))
		return (left_operand);
	token_del(&g_parser.token);
	g_parser.token = get_next_token();
	parse_linebreak(type);
	right_operand = parse_pipeline();
	if (right_operand == NULL)
	{
		if (g_parser.status == NOERR)
			g_parser.status = UNEXPECTED_TOKEN;
		command_del(&left_operand);
		return (NULL);
	}
	and_or = command_new(CONNECTION);
	and_or->value.connection->connector = type;
	and_or->value.connection->left = left_operand;
	and_or->value.connection->right = right_operand;
	return (parse_and_or_list(and_or));
}

t_command	*parse_and_or(void)
{
	return (parse_and_or_list(parse_pipeline()));
}
