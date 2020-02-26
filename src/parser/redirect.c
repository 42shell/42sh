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

static t_redir	*redir_new(t_token *from, t_token *redir_op, t_token *to)
{
	t_redir	*redir;

	redir = (t_redir *)ft_xmalloc(sizeof(t_redir));
	redir->from = from;
	redir->redir_op = redir_op;
	redir->to = to;
	return (redir);
}

/*
**	filename         : WORD
*/

static t_token	*filename(void)
{
	t_token	*filename;

	if (g_token && g_token->type == WORD)
	{
		filename = g_token;
		g_token = get_next_token();
		return (filename);
	}
	return (NULL);
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

static t_redir	*io_file(t_token *io_number)
{
	t_redir		*redir;

	redir = NULL;
	if (g_token && is_redir(g_token)
	&& (redir = redir_new(io_number, g_token, NULL)))
	{
		g_token = get_next_token();
		if (!(redir->to = filename()))
			return ((t_redir *)parse_error(NO_REDIR_FILENAME,
					redir->redir_op->value->str, redir));
	}
	return (redir);
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

static t_redir	*io_here(t_token *io_number)
{
	t_redir	*redir;

	redir = redir_new(io_number, g_token, NULL);
	if ((g_token = get_next_token()) == NULL || g_token->type != WORD)
		return ((t_redir *)parse_error(HEREDOC_NO_DELIM,
				redir->redir_op->value->str, redir));
	redir->to = g_token;
	//node_add_child(&g_heredocs, node);
	g_token = get_next_token();
	return (redir);
}

t_redir			*io_redirect(void)
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
