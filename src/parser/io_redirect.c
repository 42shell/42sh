/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_redirect.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 04:05:12 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 18:34:00 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
**	filename         : WORD
*/

static t_token	*parse_filename(void)
{
	t_token	*filename;

	if (!g_parser.token || g_parser.token->type != WORD)
		return (NULL);
	filename = g_parser.token;
	g_parser.token = get_next_token();
	return (filename);
}

/*
**  io_file          : '<'       filename
**                   | LESSAND   filename
**                   | '>'       filename
**                   | GREATAND  filename
**                   | DGREAT    filename
**                   | LESSGREAT filename
**                   ;
*/

static t_redir	*parse_io_file(t_token *io_number)
{
	t_redir		*redir;

	redir = (t_redir *)ft_xmalloc(sizeof(t_redir));
	redir->left_op = io_number;
	redir->operator = g_parser.token;
	g_parser.token = get_next_token();
	if (!(redir->right_op = parse_filename()))
	{
		if (!g_parser.status)
			g_parser.status = UNEXPECTED_TOKEN;
		redir_del(&redir);
		return (NULL);
	}
	return (redir);
}

static t_redir	*parse_io_here(t_token *io_number)
{
	t_redir	*redir;

	redir = (t_redir *)ft_xmalloc(sizeof(t_redir));
	redir->left_op = io_number;
	redir->operator = g_parser.token;
	if (!(g_parser.token = get_next_token()) || g_parser.token->type != WORD)
	{
		if (!g_parser.status)
			g_parser.status = UNEXPECTED_TOKEN;
		redir_del(&redir);
		return (NULL);
	}
	redir->right_op = g_parser.token;
	add_heredoc(redir->right_op);
	g_parser.token = get_next_token();
	return (redir);
}

t_redir			*parse_io_redirect(void)
{
	t_token *io_number;

	if (!g_parser.token)
		return (NULL);
	if (g_parser.token->type == IO_NUMBER)
	{
		io_number = g_parser.token;
		g_parser.token = get_next_token();
		if (g_parser.token && g_parser.token->type == DLESS)
			return (parse_io_here(io_number));
		return (parse_io_file(io_number));
	}
	if (!is_redir(g_parser.token))
		return (NULL);
	if (g_parser.token->type == DLESS)
		return (parse_io_here(NULL));
	return (parse_io_file(NULL));
}

t_redir			*parse_redirect_list(void)
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
