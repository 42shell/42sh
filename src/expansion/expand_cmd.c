/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 14:52:04 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/17 04:54:52 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		expand_cmd(t_command *command)
{
	dup_group_redir_list(command);
	if (expand_redir_list(command->redir_list_exp, get_var_value("HOME"))
	== AMBIG_REDIR)
	{
		ft_dprintf(2, "42sh: ambiguous redirect\n");
		return (g_last_exit_st = 1);
	}
	return (0);
}
