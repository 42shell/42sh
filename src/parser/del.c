/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/10 14:46:00 by fratajcz         ###   ########.fr       */
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
	else if (type == SUBSHELL)
		command->value.subshell = ft_xmalloc(sizeof(t_subshell));
	return (command);
}

int			redir_del(t_redir **redir)
{
	if (!redir || !*redir)
		return (0);
	redir_del(&(*redir)->next);
	token_del(&(*redir)->left_op);
	token_del(&(*redir)->operator);
	token_del(&(*redir)->right_op);
	ft_memdel((void **)redir);
	return (0);
}

int			command_del(t_command **command)
{
	if (!command || !*command)
		return (0);
	if ((*command)->type == SIMPLE)
	{
		redir_del(&(*command)->value.simple->redirs);
		token_del(&(*command)->value.simple->assigns);
		token_del(&(*command)->value.simple->args);
		free_arr((*command)->value.simple->argv);
		ft_memdel((void **)&(*command)->value.simple);
	}
	else if ((*command)->type == CONNECTION)
	{
		command_del(&(*command)->value.connection->left);
		command_del(&(*command)->value.connection->right);
		ft_memdel((void **)&(*command)->value.connection);
	}
	else if ((*command)->type == SUBSHELL)
		ft_memdel((void **)&(*command)->value.subshell);
	ft_memdel((void **)command);
	return (0);
}

void		command_list_del(t_command **command_list)
{
	t_command	*next;

	while (*command_list)
	{
		next = (*command_list)->next;
		command_del(command_list);
		*command_list = next;
	}
	*command_list = NULL;
}
