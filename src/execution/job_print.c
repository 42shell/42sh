/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/10 14:45:52 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void	format_simple_command(t_simple_cmd *command, t_dstr *buf)
{
	t_redir		*redir;
	t_token		*arg;

	redir = command->redirs;
	arg = command->args;
	while (arg != NULL)
	{
		ft_dstr_append(buf, arg->value->str);
		if ((arg = arg->next) != NULL || redir)
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

t_dstr		*format_job(t_command *command, t_dstr *buf)
{
	enum e_token_type connector;

	if (!command)
		return (NULL);
	if (!buf)
		buf = ft_dstr_new(36);
	if (command->type == CONNECTION)
	{
		format_job(command->value.connection->left, buf);
		connector = command->value.connection->connector;
		if (connector == AND_IF)
			ft_dstr_append(buf, " && ");
		else if (connector == OR_IF)
			ft_dstr_append(buf, " || ");
		else if (connector == PIPE)
			ft_dstr_append(buf, " | ");
		format_job(command->value.connection->right, buf);
	}
	else if (command->type == SIMPLE)
		format_simple_command(command->value.simple, buf);
	return (buf);
}

void		print_job(t_job *job, int status)
{
	t_dstr	*format;
	char	*status_str;
	char	*curr;
	
	status_str = NULL;
	if (status == JOB_DONE)
		status_str = "Done";
	else if (status == JOB_STOPPED)
		status_str = "Stopped";
	if (job == g_shell.curr_job)
		curr = ft_strdup("+");
	else if (job == g_shell.prev_job)
		curr = ft_strdup("-");
	else
		curr = ft_strdup(" ");
	format = format_job(job->command, NULL);
	ft_printf("[%d]%s %s %s\n", job->id + 1, curr, status_str, format->str);
	ft_dstr_del(&format);
	free(curr);
}
