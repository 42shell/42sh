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
		if (redir->left_op)
			ft_dstr_append(buf, redir->left_op->value->str);
		ft_dstr_append(buf, redir->operator->value->str);
		ft_dstr_append(buf, redir->right_op->value->str);
		if ((redir = redir->next))
			ft_dstr_append(buf, " ");
	}
}

t_dstr		*format_command(t_command *command, t_dstr *buf)
{
	enum e_token_type connector;

	if (!command)
		return (NULL);
	if (command->type == CONNECTION)
	{
		format_command(command->value.connection->left, buf);
		connector = command->value.connection->connector;
		if (connector == AND_IF)
			ft_dstr_append(buf, " && ");
		else if (connector == OR_IF)
			ft_dstr_append(buf, " || ");
		else if (connector == PIPE)
			ft_dstr_append(buf, " | ");
		format_command(command->value.connection->right, buf);
	}
	else if (command->type == SIMPLE)
		format_simple_command(command->value.simple, buf);
	return (buf);
}

void		format_process_long(t_process *process, t_dstr *buf)
{
	char	*pid;
	char	*sig;
	char	*exit_st;

	sig = NULL;
	exit_st = NULL;
	pid = ft_itoa(process->pid);
	ft_dstr_append(buf, pid);
	ft_dstr_append(buf, " ");
	if (WIFEXITED(process->status))
	{
		exit_st = ft_itoa(WEXITSTATUS(process->status));
		ft_dstr_append(buf, "Exit ");
		ft_dstr_append(buf, exit_st);
		ft_dstr_append(buf, "       ");
	}
	else if (WIFSTOPPED(process->status))
	{
		ft_dstr_append(buf, "Stopped ");
		sig = ft_itoa(WSTOPSIG(process->status));
		ft_dstr_append(buf, sig);
		ft_dstr_append(buf, "   ");
	}
	else
		ft_dstr_append(buf, "Running       ");
	if (process->stdin != 0)
		ft_dstr_append(buf, "| ");
	format_command(process->command, buf);
	free(exit_st);
	free(sig);
	free(pid);
}

/*
[1]+  Stopped                 cat | test
[1]+    31 Stopped (tty input)     cat
        32 Exit 1                  | test
*/

void		print_job_long(t_job *job)
{
	t_dstr		*buf;
	t_process	*process;
	char		*curr;

	buf = ft_dstr_new(32);
	process = job->processes;
	while (process)
	{
		format_process_long(process, buf);
		if (process->next)
			ft_dstr_append(buf, "\n       ");
		process = process->next;
	}
	if (job == g_shell.curr_job)
		curr = "+";
	else if (job == g_shell.prev_job)
		curr = "-";
	else
		curr = " ";
	ft_printf("[%d]%s   %s\n", job->id + 1, curr, buf->str);
}


void		print_job(t_job *job)
{
	t_dstr	*format;
	char	*status_str;
	char	*curr;
	
	status_str = NULL;
	if (job_is_done(job))
		status_str = "Done";
	else if (job_is_stopped(job))
		status_str = "Stopped";
	else
		status_str = "Running";
	if (job == g_shell.curr_job)
		curr = "+";
	else if (job == g_shell.prev_job)
		curr = "-";
	else
		curr = " ";
	format = format_command(job->command, NULL);
	ft_printf("[%d]%s %-20s %s\n", job->id + 1, curr, status_str, format->str);
	ft_dstr_del(&format);
	free(curr);
}