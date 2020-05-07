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

void		format_simple_command(t_dstr *buf, t_simple_cmd *command)
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
		if (redir->left_op)
			ft_dstr_append(buf, redir->left_op->value->str);
		ft_dstr_append(buf, redir->operator->value->str);
		ft_dstr_append(buf, redir->right_op->value->str);
		if ((redir = redir->next))
			ft_dstr_append(buf, " ");
	}
}

t_dstr		*format_command(t_dstr *buf, t_command *command)
{
	enum e_token_type connector;

	if (!command || !buf)
		return (NULL);
	if (command->type == CONNECTION)
	{
		format_command(buf, command->value.connection->left);
		connector = command->value.connection->connector;
		if (connector == AND_IF)
			ft_dstr_append(buf, " && ");
		else if (connector == OR_IF)
			ft_dstr_append(buf, " || ");
		else if (connector == PIPE)
			ft_dstr_append(buf, " | ");
		format_command(buf, command->value.connection->right);
	}
	else if (command->type == SIMPLE)
		format_simple_command(buf, command->value.simple);
	return (buf);
}

void		print_job_long(t_job *job)
{
	t_dstr		*command_format;
	char		*process_format;
	t_process	*process;

	if (!job)
		return ;
	print_job(job, false);
	command_format = ft_dstr_new(64);
	process = job->processes;
	while (process)
	{
		process_format = get_process_format(process);
		if (process->stdin != 0)
			ft_dstr_append(command_format, "| ");
		format_command(command_format, process->command);
		ft_printf("     %-30s %s\n", process_format, command_format->str);
		ft_dstr_clear(command_format, 64);
		free(process_format);
		process = process->next;
	}
	ft_dstr_del(&command_format);
}

void		print_job(t_job *job, bool print_command)
{
	t_dstr	*command_format;
	char	*job_format;
	char	*curr;
	
	if (!job)
		return ;
	if (is_current_job(job))
		curr = "+";
	else if (is_previous_job(job))
		curr = "-";
	else
		curr = " ";
	job_format = get_job_format(job);
	if (print_command)
	{
		command_format = ft_dstr_new(64);
		format_command(command_format, job->command);
	}
	ft_printf("[%d]%s %-30s %s\n", job->id + 1, curr, job_format,
	print_command ? command_format->str : "");
	if (print_command)
		ft_dstr_del(&command_format);
	free(job_format);
}