/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 08:48:18 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 18:33:46 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"


#include <stdio.h>
void	print_command(t_command *command, int indent)
{
	t_token		*arg;
	t_redir		*redir;
	int			i;

	i = 0;
	if (!command)
		return ;
	if (command->flags & CMD_INVERT_RETURN)
	{
		while (i++ < indent)
			printf("    ");
		printf("%s\n", "!");
	}
	if (command->type == SIMPLE)
	{
		while (i++ < indent)
			printf("    ");
		arg = command->value.simple->args;
		redir = command->value.simple->redirs;
		while (arg)
		{
			printf("%s ", arg->value->str);
			arg = arg->next;
		}
		while (redir)
		{
			if (redir->left_op)
				printf("%s", redir->left_op->value->str);
			printf("%s", redir->operator->value->str);
			printf("%s ", redir->right_op->value->str);
			redir = redir->next;
		}
		printf("\n");
		return ;
	}
	if (command->type == CONNECTION)
	{
		while (i++ < indent)
			printf("    ");
		indent++;
		printf("%s\n", command->value.connection->connector == PIPE ?
		"|" : command->value.connection->connector == AND_IF ? "&&" : "||");
		print_command(command->value.connection->left, indent);
		print_command(command->value.connection->right, indent);
		return ;
	}
	if (command->type == GROUP)
	{
		while (i++ < indent)
			printf("    ");
		printf("%s ", command->value.group->subshell ? "( " : "{ ");
		while (command->value.group->list)
		{
			print_command(command->value.group->list, indent);
			command->value.group->list = command->value.group->list->next;
		}
		i = 0;
		while (i++ < indent)
			printf("    ");
		printf("%s ", command->value.group->subshell ? ")\n" : "}\n");
		return ;
	}
	if (command->type == IF_CLAUSE)
	{
		if (command->value.if_clause->if_part)
		{
			while (i++ < indent)
				printf("    ");
			printf("IF\n");
		}
		while (command->value.if_clause->if_part)
		{
			print_command(command->value.if_clause->if_part, indent);
			command->value.if_clause->if_part
			= command->value.if_clause->if_part->next;
		}
		i = 0;
		while (i++ < indent)
			printf("    ");
		printf("THEN\n");
		while (command->value.if_clause->then_part)
		{
			print_command(command->value.if_clause->then_part, indent);
			command->value.if_clause->then_part
			= command->value.if_clause->then_part->next;
		}
		if (command->value.if_clause->else_part)
		{
			i = 0;
			while (i++ < indent)
				printf("    ");
			printf("ELSE\n");
			print_command(command->value.if_clause->else_part, indent);
		}
		return ;
	}
}
