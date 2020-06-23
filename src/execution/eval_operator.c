/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_operator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/05 15:54:13 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			eval_pipeline(t_command *command, int in, int out)
{
	t_connection	*pipeline;
	t_process		*process;
	int				fd[2];

	if (pipe(fd) == -1)
		return (-1);
	pipeline = command->value.connection;
	process = process_new(pipeline->right, fd[0], out);
	launch_process(process, fd[1]);
	if (pipeline->left->type == CONNECTION
	&& pipeline->left->value.connection->connector == PIPE)
		return (eval_pipeline(pipeline->left, in, fd[1]));
	process = process_new(pipeline->left, in, fd[1]);
	return (launch_process(process, fd[0]));
}

static int	wait_for_and_or(t_connection *and_or)
{
	if (!g_bg && g_job_control_enabled)
		put_job_fg(g_shell.jobs, false);
	else
		wait_for_job(g_shell.jobs);
	if (and_or->left->flags & CMD_INVERT_RETURN)
		g_last_exit_st = g_last_exit_st ? 0 : 1;
	return (g_last_exit_st);
}

int			eval_and_or(t_command *command)
{
	t_connection	*and_or;

	g_already_forked = false;
	and_or = command->value.connection;
	eval_command(and_or->left);
	wait_for_and_or(and_or);
	if ((and_or->connector == AND_IF && g_last_exit_st == 0)
	|| (and_or->connector == OR_IF && g_last_exit_st != 0))
		eval_command(and_or->right);
	wait_for_and_or(and_or);
	return (0);
}
