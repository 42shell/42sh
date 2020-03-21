/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/14 17:52:18 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			close_pipe(t_process *process)
{
	if (process->stdin != STDIN_FILENO)
		close(process->stdin);
	if (process->stdout != STDOUT_FILENO)
		close(process->stdout);
	return (0);
}

int			set_pipe(t_process *process)
{
	int		fd[2];

	if ((pipe(fd)) == -1)
		return (-1);
    process->stdout = fd[1];
	process->next->stdin = fd[0];
	return (0);
}