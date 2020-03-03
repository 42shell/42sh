/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
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
	token_del(&(*redir)->left_op);
	token_del(&(*redir)->operator);
	token_del(&(*redir)->right_op);
	ft_memdel((void **)redir);
	return (0);
}

t_redir		*redir_new(t_token *left_op, t_token *operator, t_token *right_op)
{
	t_redir	*redir;

	redir = (t_redir *)ft_xmalloc(sizeof(t_redir));
	redir->left_op = left_op;
	redir->operator = operator;
	redir->right_op = right_op;
	return (redir);
}

int			is_command(t_node *node) //node->type
{
	return (node->data && node->nb_children == 0);
}

int			command_del(t_command **command)
{
	size_t	i;

	i = 0;
	if (!command || !*command)
		return (0);
	while ((*command)->argv && (*command)->argv[i])
		token_del(&(*command)->argv[i++]);
	i = 0;
	while ((*command)->redirs && (*command)->redirs[i])
		redir_del(&(*command)->redirs[i++]);
	free((*command)->argv);
	free((*command)->redirs);
	ft_memdel((void **)command);
	return (0);
}

t_command	*command_new(void)
{
	return ((t_command *)ft_xmalloc(sizeof(t_command)));
}
