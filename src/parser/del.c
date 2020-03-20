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

int			complete_command_del(t_complete_command **complete_command)
{
	if (!complete_command || !*complete_command)
		return (0);
	complete_command_del(&(*complete_command)->next);
	job_del(&(*complete_command)->jobs);
	ft_memdel((void **)complete_command);
	return (0);
}

int			process_del(t_process **process)
{
	if (!process || !*process)
		return (0);
	process_del(&(*process)->next);
	token_del(&(*process)->words);
	redir_del(&(*process)->redirs);
	free((*process)->path);
	ft_memdel((void **)process);
	return (0);
}

int			pipeline_del(t_pipeline **pipeline)
{
	if (!pipeline || !*pipeline)
		return (0);
	pipeline_del(&(*pipeline)->next);
	process_del(&(*pipeline)->processes);
	token_del(&(*pipeline)->sep);
	ft_memdel((void **)pipeline);
	return (0);
}

int			job_del(t_job **job)
{
	if (!job || !*job)
		return (0);
	job_del(&(*job)->next);
	pipeline_del(&(*job)->pipelines);
	token_del(&(*job)->sep);
	ft_memdel((void **)job);
	return (0);
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