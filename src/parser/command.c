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

static void	add_command_arg(t_command *command, t_token *arg)
{
	t_token		**new;
	int			size;

	if (!command->argv)
	{
		command->argv = (t_token **)ft_xmalloc(sizeof(t_token *) * 2);
		command->argv[0] = arg;
		return ;
	}
	size = 0;
	while (command->argv[size])
		size++;
	new = (t_token **)ft_xmalloc(sizeof(t_token *) * (size + 2));
	ft_memcpy((char *)new, (char *)command->argv, (size * sizeof(t_token *)));
	new[size] = arg;
	free(command->argv);
	command->argv = new;
}

static void	add_command_redir(t_command *command, t_redir *redir)
{
	t_redir		**new;
	int			size;

	if (!command->redirs)
	{
		command->redirs = (t_redir **)ft_xmalloc(sizeof(t_redir *) * 2);
		command->redirs[0] = redir;
		return ;
	}
	size = 0;
	while (command->redirs[size])
		size++;
	new = (t_redir **)ft_xmalloc(sizeof(t_redir *) * (size + 2));
	ft_memcpy((char *)new, (char *)command->redirs, (size * sizeof(t_token *)));
	new[size] = redir;
	free(command->redirs);
	command->redirs = new;
}

t_node			*command(void)
{
	t_node		*command_node;
	t_command	*command;
	t_redir		*redirect;

	if (g_parser.error)
		return (NULL);
	command = command_new();
	command_node = node_new(command);
	while (!g_parser.error && g_parser.token
	&& ((redirect = io_redirect()) || g_parser.token->type == WORD))
	{
		if (redirect)
			add_command_redir(command, redirect);
		else
		{
			add_command_arg(command, g_parser.token);
			g_parser.token = get_next_token();
		}
	}
	if (g_parser.error || (!command->argv && !command->redirs))
	{
		command_del(&command);
		ft_memdel((void **)&command_node);
	}
	return (command_node);
}
