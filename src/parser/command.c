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
**		type = 		NODE_COMMAND
** 		data =		t_command
** 					{
** 						char 	**argv		= { ls, -l, -a, -f };
** 						t_redir **redirs	= { 2, >, test };
						char	*path		= /bin/ls
** 					}
** 		childs = 	NULL;
** }
*/

static void	add_command_arg(t_command *command, t_token *arg)
{
	char		**new;
	int			size;

	if (!command->argv)
	{
		command->argv = (char **)ft_xmalloc(sizeof(char *) * 2);
		command->argv[0] = ft_strdup(arg->value->str);
		return ;
	}
	size = 0;
	while (command->argv[size])
		size++;
	new = (char **)ft_xmalloc(sizeof(char *) * (size + 2));
	ft_memcpy((char *)new, (char *)command->argv, (size * sizeof(char *)));
	new[size] = ft_strdup(arg->value->str);
	free(command->argv);
	command->argv = new;
	token_del(&arg);
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
	command_node = node_new(command, NODE_COMMAND);
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
