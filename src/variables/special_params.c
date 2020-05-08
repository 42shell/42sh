/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_params.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz </var/spool/mail/fratajcz>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/06 17:23:15 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/07 15:33:57 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** value func for $?
*/

char	*get_last_exit_status(void)
{
	static char	str[10];

	return (ft_itoa(g_last_exit_st, str));
}

char	g_shell_pid[10];

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

/*
** $!
** Expands to the process ID of the job most recently placed into the background
** whether executed as an asynchronous command or using the bg builtin
*/

char	*get_last_bg_job_pid(void)
{
	static char	str[10];

	if (g_last_bg_job_pid == -1)
		return ("");
	return (ft_itoa(g_last_bg_job_pid, str));
}
