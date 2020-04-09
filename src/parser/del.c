/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/09 17:59:02 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_command	*command_new(enum e_cmd_type type)
{
	t_command	*command;

	command = ft_xmalloc(sizeof(t_command));
	command->type = type;
	if (type == SIMPLE)
	{
		command->value.simple = ft_xmalloc(sizeof(t_simple_cmd));
		command->value.simple->argv = array_new();
	}
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
		token_del(&(*command)->value.simple->assign_list);
		array_destroy((*command)->value.simple->argv);
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

/*
int			pipeline_del(t_pipeline **pipeline)
{
	if (!pipeline || !*pipeline)
		return (0);
	pipeline_del(&(*pipeline)->next);
	command_del(&(*pipeline)->commands);
	ft_memdel((void **)pipeline);
	return (0);
}

int			and_or_del(t_and_or **and_or)
{
	if (!and_or || !*and_or)
		return (0);
	and_or_del(&(*and_or)->next);
	pipeline_del(&(*and_or)->pipelines);
	ft_memdel((void **)and_or);
	return (0);
}
*/

int			ast_del(t_node **ast)
{
	if (!ast || !*ast)
		return (0);
	if ((*ast)->type == NODE_SMPL_CMD)
		command_del((t_command **)&(*ast)->data);
	ast_del(&(*ast)->left);
	ast_del(&(*ast)->right);
	free(*ast);
	*ast = NULL;
	return (0);
}

/*
int			list_del(t_list **list)
{
	if (!list || !*list)
		return (0);
	list_del(&(*list)->next);
	ast_del(&(*list)->ast);
	ft_memdel((void **)list);
	return (0);
}*/
