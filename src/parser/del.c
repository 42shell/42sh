/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/13 18:32:06 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

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
	token_del(&(*command)->words);
	redir_del(&(*command)->redirs);
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