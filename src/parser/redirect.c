/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 04:05:12 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/13 18:32:33 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
**	filename         : WORD
*/

static t_node	*filename(void)
{
	t_node *node;

	node = NULL;
	if (g_token && g_token->type == WORD)
	{
		node = node_new(g_token, 0);
		g_token = get_next_token();
	}
	return (node);
}

/*
**  io_file          : '<'       filename
**                   | LESSAND   filename
**                   | '>'       filename
**                   | GREATAND  filename
**                   | DGREAT    filename
**                   | LESSGREAT filename
**                   ;
**
**	the io_number is added as child 0 of the redirect node, and the output is
**	added as child 1
**	example:               2> file.txt
**
**                           >
**                          / \
**                         2  file.txt
**
**  if there is no input file descriptor, the input node's data is set to NULL
**	like a command node (not ideal)
*/

static t_node	*io_file(t_token *io_number)
{
	t_node		*node;
	t_node		*filename_node;

	node = NULL;
	if (g_token && is_redir(g_token) && (node = node_new(g_token, 0)))
	{
		g_token = get_next_token();
		if (!(filename_node = filename()))
		{
			token_del(&io_number);
			return (parse_error(NO_REDIR_FILENAME,
					node_token(node)->value->str, node));
		}
		node_add_child(node, node_new(io_number, 0));
		node_add_child(node, filename_node);
	}
	return (node);
}

/*
**	the IO_NUMBER is added as child 0 of the heredoc node, and the delimiter
**	is added as child 1. If there is no IO_NUMBER, child 0 has NULL data.
**	example:				cat 1 << EOF
**
**                              <<
**                             /  \
**                            1   EOF
*/

static t_node	*io_here(t_token *io_number)
{
	t_node	*node;

	node = node_new(g_token, 0);
	g_token = get_next_token();
	if (g_token == NULL || g_token->type != WORD)
		return (parse_error(HEREDOC_NO_DELIM,
				node_token(node)->value->str, node));
	node_add_child(node, node_new(io_number, 0));
	node_add_child(node, node_new(g_token, 0));
	//node_add_child(&g_heredocs, node);
	g_token = get_next_token();
	return (node);
}

t_node			*io_redirect(void)
{
	t_token *io_number;

	if (g_token && g_token->type == IO_NUMBER)
	{
		io_number = g_token;
		g_token = get_next_token();
		if (g_token && (g_token->type == DLESS))
			return (io_here(io_number));
		return (io_file(io_number));
	}
	else if (g_token && (g_token->type == DLESS))
		return (io_here(NULL));
	return (io_file(NULL));
}
