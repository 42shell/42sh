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

int			process_del(t_process **process)
{
	size_t	i;

	i = 0;
	if (!process || !*process)
		return (0);
	free_arr((*process)->argv);
	while ((*process)->redirs && (*process)->redirs[i])
		redir_del(&(*process)->redirs[i++]);
	free((*process)->redirs);
	free((*process)->path);
	ft_memdel((void **)process);
	return (0);
}

t_process	*process_new(void)
{
	t_process	*cmd;

	cmd = (t_process *)ft_xmalloc(sizeof(t_process));
	/*
	cmd->stdin = STDIN_FILENO;
	cmd->stdout = STDOUT_FILENO;
	*/
	return (cmd);
}
