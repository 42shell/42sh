/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/14 17:52:18 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void	format_command(t_node *node, t_dstr *buf)
{
	t_command	*command;
	t_token		*word;
	t_redir		*redir;

	command = (t_command *)node->data; 
	word = command->words;
	redir = command->redirs;
	while (word)
	{
		ft_dstr_append(buf, word->value->str);
		if ((word = word->next) || redir)
			ft_dstr_append(buf, " ");
	}
	while (redir)
	{
		ft_dstr_append(buf, redir->left_op->value->str);
		ft_dstr_append(buf, redir->operator->value->str);
		ft_dstr_append(buf, redir->right_op->value->str);
		if ((redir = redir->next))
			ft_dstr_append(buf, " ");
	}
}

t_dstr		*format_job(t_node *node, t_dstr *buf)
{
	if (!node)
		return (NULL);
	if (!buf)
		buf = ft_dstr_new(36);
	format_job(node->left, buf);
	if (node->type == NODE_AND)
		ft_dstr_append(buf, " && ");
	else if (node->type == NODE_OR)
		ft_dstr_append(buf, " || ");
	else if (node->type == NODE_PIPE)
		ft_dstr_append(buf, " | ");
	else if (node->type == NODE_SMPL_CMD)
		format_command(node, buf);
	format_job(node->right, buf);
	return (buf);
}

void		print_job(t_job *job, int status)
{
	t_dstr	*format;
	char	*status_str;

	status_str = NULL;
	if (status == JOB_DONE)
		status_str = "Done";
	else if (status == JOB_STOPPED)
		status_str = "Stopped";
	format = format_job(job->ast, NULL);
	ft_printf("[%d]+  %s    %s\n", job->id + 1, status_str, format->str);
	ft_dstr_del(&format);
}