/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 18:35:38 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static t_redir	*parse_redirect_list(void)//file
{
	t_redir		*list;
	t_redir		*curr;
	t_redir		*redir;

	list = NULL;
	while ((redir = parse_io_redirect()))
	{
		if (!list)
		{
			list = redir;
			curr = list;
		}
		else
		{
			curr->next = redir;
			curr = curr->next;
		}
	}
	return (list);
}

/*
** command         : simple_command
**                 | compound_command
**                 | compound_command redirect_list
*/

t_command		*parse_command(void)
{
	t_command	*command;

	if (!(command = parse_simple_command()))
	{
		if ((command = parse_compound_command()))
			command->redir_list = parse_redirect_list();
	}
	return (command);
}
