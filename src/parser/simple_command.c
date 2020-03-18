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

static void	add_process_arg(t_process *process, t_token *arg)
{
	char		**new;
	int			size;

	if (!process->argv)
	{
		process->argv = (char **)ft_xmalloc(sizeof(char *) * 2);
		process->argv[0] = ft_strdup(arg->value->str);
		return ;
	}
	size = 0;
	while (process->argv[size])
		size++;
	new = (char **)ft_xmalloc(sizeof(char *) * (size + 2));
	ft_memcpy((char *)new, (char *)process->argv, (size * sizeof(char *)));
	new[size] = ft_strdup(arg->value->str);
	free(process->argv);
	process->argv = new;
	token_del(&arg);
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

t_node		*simple_command(void)
{
	t_node		*process_node;
	t_process	*process;
	t_redir		*redirect;

	if (g_parser.error)
		return (NULL);
	process = process_new();
	process_node = node_new(process, NODE_PROCESS);
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
		ft_memdel((void **)&process_node);
	}
	return (process_node);
}
