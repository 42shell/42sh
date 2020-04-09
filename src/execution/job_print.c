/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/09 23:49:32 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void	format_simple_command(t_simple_cmd *command, t_dstr *buf)
{
	char		*arg;
	t_redir		*redir;

	arg = command->argv->array[0];
	redir = command->redirs;
	while (arg)
	{
		ft_dstr_append(buf, arg);
		if ((arg = arg + 1) || redir)
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
		else if (connector == AND_IF)
			ft_dstr_append(buf, " || ");
		else if (connector == PIPE)
			ft_dstr_append(buf, " | ");
		format_job(command->value.connection->right, buf);
	}
	else if (command->type == SIMPLE)
		format_simple_command(command->value.simple, buf);
	//subshell
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
	format = format_job(job->command, NULL);
	ft_printf("[%d]+  %s    %s\n", job->id + 1, status_str, format->str);
	ft_dstr_del(&format);
}
