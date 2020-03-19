/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/13 18:32:06 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			free_processes(t_process **processes)
{
	if (!processes || !*processes)
		return (0);
	free_processes(&(*processes)->next);
	free_arr((*processes)->argv);
	free_redirs(&(*processes)->redirs);
	free((*processes)->path);
	ft_memdel((void **)processes);
	return (0);
}

int			free_pipelines(t_pipeline **pipelines)
{
	if (!pipelines || !*pipelines)
		return (0);
	free_pipelines(&(*pipelines)->next);
	free_processes(&(*pipelines)->processes);
	token_del(&(*pipelines)->sep);
	ft_memdel((void **)pipelines);
	return (0);
}

int			free_jobs(t_job **jobs)
{
	if (!jobs || !*jobs)
		return (0);
	free_jobs(&(*jobs)->next);
	free_pipelines(&(*jobs)->pipelines);
	token_del(&(*jobs)->sep);
	ft_memdel((void **)jobs);
	return (0);
}

int			free_redirs(t_redir **redirs)
{
	if (!redirs || !*redirs)
		return (0);
	free_redirs(&(*redirs)->next);
	token_del(&(*redirs)->left_op);
	token_del(&(*redirs)->operator);
	token_del(&(*redirs)->right_op);
	ft_memdel((void **)redirs);
	return (0);
}