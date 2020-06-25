/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_del.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/26 02:27:24 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_command	*command_new(enum e_cmd_type type)
{
	t_command	*command;

	command = ft_xmalloc(sizeof(t_command));
	command->type = type;
	if (type == SIMPLE)
		command->value.simple = ft_xmalloc(sizeof(t_simple_cmd));
	else if (type == CONNECTION)
		command->value.connection = ft_xmalloc(sizeof(t_connection));
	else if (type == GROUP)
		command->value.group = ft_xmalloc(sizeof(t_group_cmd));
	else if (type == IF_CLAUSE)
		command->value.if_clause = ft_xmalloc(sizeof(t_if_clause));
	return (command);
}

void		redir_del(t_redir **redir)
{
	if (!redir || !*redir)
		return ;
	redir_del(&(*redir)->next);
	token_del(&(*redir)->left_op);
	token_del(&(*redir)->operator);
	token_del(&(*redir)->right_op);
	ft_memdel((void **)redir);
}

void		simple_command_del(t_simple_cmd **simple)
{
	redir_del(&(*simple)->redirs);
	token_list_del(&(*simple)->assigns);
	token_list_del(&(*simple)->args);
	free_arr((*simple)->argv);
	ft_memdel((void **)simple);
}

void		command_del(t_command **command)
{
	if (!command || !*command)
		return ;
	if ((*command)->type == SIMPLE)
		simple_command_del(&(*command)->value.simple);
	else if ((*command)->type == CONNECTION)
	{
		command_del(&(*command)->value.connection->left);
		command_del(&(*command)->value.connection->right);
		ft_memdel((void **)&(*command)->value.connection);
	}
	else if ((*command)->type == GROUP)
	{
		complete_command_del(&(*command)->value.group->list);
		ft_memdel((void **)&(*command)->value.group);
	}
	else if ((*command)->type == IF_CLAUSE)
	{
		complete_command_del(&(*command)->value.if_clause->if_part);
		complete_command_del(&(*command)->value.if_clause->then_part);
		complete_command_del(&(*command)->value.if_clause->else_part);
		ft_memdel((void **)&(*command)->value.if_clause);
	}
	redir_del(&((*command)->redir_list));
	ft_memdel((void **)command);
}

void		complete_command_del(t_command **complete_command)
{
	t_command	*next;

	while (*complete_command)
	{
		next = (*complete_command)->next;
		command_del(complete_command);
		*complete_command = next;
	}
	*complete_command = NULL;
}
