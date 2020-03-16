/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 15:49:39 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/24 15:50:33 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		set_pipe_redir(int input_fd, int fildes[2])
{
	if (dup2(fildes[1], 1) == -1 || dup2(input_fd, 0) == -1)
		return (-1);
	close(fildes[1]);
	close(fildes[0]);
	if (input_fd != 0)
		close(input_fd);
	return (0);
}

void	kill_all_forks(void)
{
	write(2, "42sh: too many processes, aborting\n", 35);
	kill(0, SIGABRT);
}

void	interrupt_fork(int sig)
{
	if (sig == SIGINT)
		write(1, "\n", 1);
	g_last_exit_st = 130;
}