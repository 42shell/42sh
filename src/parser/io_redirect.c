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

static void		ps_add_heredoc(t_token *heredoc)
{
	t_token		*ptr;

	if (!g_parser.heredocs)
		g_parser.heredocs = heredoc;
	else
	{
		ptr = g_parser.heredocs;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = heredoc;
	}
	/*
	t_token		**new;
	int			size;

	if (!g_parser.heredocs)
	{
		g_parser.heredocs = (t_token **)ft_xmalloc(sizeof(t_token *) * 2);
		g_parser.heredocs[0] = heredoc;
		return ;
	}
	size = 0;
	while (g_parser.heredocs[size])
		size++;
	new = (t_token **)ft_xmalloc(sizeof(t_token *) * (size + 2));
	ft_memcpy((char *)new, (char *)g_parser.heredocs, (size * sizeof(t_token *)));
	new[size] = heredoc;
	free(g_parser.heredocs);
	g_parser.heredocs = new;
	*/
}

/*
**	filename         : WORD
*/

static t_token	*ps_filename(void)
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

static t_redir	*ps_io_file(t_token *io_number)
{
	t_redir		*redir;

	redir = (t_redir *)ft_xmalloc(sizeof(t_redir));
	redir->left_op = io_number;
	redir->operator = g_parser.token;
	g_parser.token = get_next_token();
	if (!(redir->right_op = ps_filename()))
	{
		g_parser.error = NO_REDIR_FILENAME;
		redir_del(&redir);
		return (NULL);
	}
	return (redir);
}

static t_redir	*ps_io_here(t_token *io_number)
{
	t_redir	*redir;

	redir = (t_redir *)ft_xmalloc(sizeof(t_redir));
	redir->left_op = io_number;
	redir->operator = g_parser.token;
	if (!(g_parser.token = get_next_token()) || g_parser.token->type != WORD)
	{
		g_parser.error = HEREDOC_NO_DELIM;
		redir_del(&redir);
		return (NULL);
	}
	redir->right_op = g_parser.token;
	ps_add_heredoc(redir->right_op);
	g_parser.token = get_next_token();
	return (redir);
}

t_redir			*ps_io_redirect(void)
{
	t_token *io_number;

	if (g_parser.token->type == IO_NUMBER)
	{
		io_number = g_parser.token;
		g_parser.token = get_next_token();
		if (g_parser.token && g_parser.token->type == DLESS)
			return (ps_io_here(io_number));
		return (ps_io_file(io_number));
	}
	else if (g_parser.token->type == DLESS)
		return (ps_io_here(NULL));
	return (ps_io_file(NULL));
}
