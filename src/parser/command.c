/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/13 18:32:06 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** returns commands in this format:
**
** ls -l 2 > test -a -f
**
** t_node
** {
** 		data =		t_process
** 					{
** 						t_token **argv		= { ls, -l, -a, -f };
** 						t_redir **redirs	= { 2> };
** 					}
** 		childs = 	NULL;
** }
*/

static void	add_process_arg(t_process *process, t_token *arg)
{
	t_token		**new;
	int			size;

	if (!process->argv)
	{
		process->argv = (t_token **)ft_xmalloc(sizeof(t_token *) * 2);
		process->argv[0] = arg;
		return ;
	}
	size = 0;
	while (process->argv[size])
		size++;
	new = (t_token **)ft_xmalloc(sizeof(t_token *) * (size + 2));
	ft_memcpy((char *)new, (char *)process->argv, (size * sizeof(t_token *)));
	new[size] = arg;
	free(process->argv);
	process->argv = new;
}

static void	add_process_redir(t_process *process, t_redir *redir)
{
	t_redir		**new;
	int			size;

	if (!process->redirs)
	{
		process->redirs = (t_redir **)ft_xmalloc(sizeof(t_redir *) * 2);
		process->redirs[0] = redir;
		return ;
	}
	size = 0;
	while (process->redirs[size])
		size++;
	new = (t_redir **)ft_xmalloc(sizeof(t_redir *) * (size + 2));
	ft_memcpy((char *)new, (char *)process->redirs, (size * sizeof(t_token *)));
	new[size] = redir;
	free(process->redirs);
	process->redirs = new;
}

t_node			*command(void)
{
	t_node		*command_node;
	t_process	*process;
	t_redir		*redirect;

	if (g_parser.error)
		return (NULL);
	process = process_new();
	command_node = node_new(process);
	while (!g_parser.error && g_parser.token
	&& ((redirect = io_redirect()) || g_parser.token->type == WORD))
	{
		if (redirect)
			add_process_redir(process, redirect);
		else
		{
			add_process_arg(process, g_parser.token);
			g_parser.token = get_next_token();
		}
	}
	if (g_parser.error || (!process->argv && !process->redirs))
	{
		process_del(&process);
		ft_memdel((void **)&command_node);
	}
	return (command_node);
}
