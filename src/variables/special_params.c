/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_params.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/08 18:37:29 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 18:37:30 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** value func for $?
*/

char	*get_last_exit_status(void)
{
	static char	str[12];

	return (ft_itoa(g_last_exit_st, str));
}

char	g_shell_pid[12];

/*
** $$
** returns the pid of the shell
** In a subshell, returns the pid of the invoking shell
*/

char	*get_shell_pid(void)
{
	return (g_shell_pid);
}

int		g_last_bg_job_pid = -1;
int		g_last_bg_job_pgid = -1;

/*
** $!
** Expands to the process ID of the command most recently placed
** into the background whether executed as an asynchronous command or using
** the bg builtin
*/

char	*get_last_bg_job_pid(void)
{
	static char	str[12];

	if (g_last_bg_job_pid == -1)
		return ("");
	return (ft_itoa(g_last_bg_job_pid, str));
}

/*
** $!!
** Expands to the process group ID of the job most recently placed
** into the background whether executed as an asynchronous command or using
** the bg builtin
*/

char	*get_last_bg_job_pgid(void)
{
	static char	str[12];

	if (g_last_bg_job_pgid == -1)
		return ("");
	return (ft_itoa(g_last_bg_job_pgid, str));
}
