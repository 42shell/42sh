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

extern int		g_parse_error;
extern char		*g_error_near;
extern t_node	g_heredocs;

/*
**	filename         : WORD
*/

static t_node	*filename(t_lexer *lexer)
{
	t_node *node;

	if (lexer->curr_tok == NULL || g_parse_error != NOERR)
		return (NULL);
	node = NULL;
	if (lexer->curr_tok->type == WORD)
	{
		node = node_new(lexer->curr_tok);
		eat(lexer);
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

static t_node	*io_file(t_lexer *lexer, t_token *io_number)
{
	t_node		*node;
	t_node		*filename_node;

	if (lexer->curr_tok == NULL || g_parse_error != NOERR)
		return (NULL);
	node = NULL;
	if (is_redir(lexer->curr_tok) && (node = node_new(lexer->curr_tok)))
	{
		eat(lexer);
		filename_node = filename(lexer);
		if (filename_node == NULL)
		{
			if (g_parse_error == 0)
			{
				g_parse_error = NO_REDIR_FILENAME;
				g_error_near = ft_strdup(node_token(node)->value->str);
			}
			token_del(&io_number);
			free_ast_nodes(node, false);
			return (NULL);
		}
		node_add_child(node, node_new(io_number));
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

static t_node	*io_here(t_lexer *lexer, t_token *io_number)
{
	t_node	*node;

	if (g_parse_error != NOERR)
		return (NULL);
	node = NULL;
	node = node_new(lexer->curr_tok);
	eat(lexer);
	if (lexer->curr_tok == NULL || lexer->curr_tok->type != WORD)
	{
		g_parse_error = HEREDOC_NO_DELIM;
		g_error_near = ft_strdup(node_token(node)->value->str);
		free_ast_nodes(node, false);
		return (NULL);
	}
	node_add_child(node, node_new(io_number));
	node_add_child(node, node_new(lexer->curr_tok));
	node_add_child(&g_heredocs, node);
	eat(lexer);
	return (node);
}

t_node			*io_redirect(t_lexer *lexer)
{
	t_token *io_number;

	if (lexer->curr_tok == NULL || g_parse_error != NOERR)
		return (NULL);
	if (lexer->curr_tok->type == IO_NUMBER)
	{
		io_number = lexer->curr_tok;
		eat(lexer);
		if (lexer->curr_tok && (lexer->curr_tok->type == DLESS))
			return (io_here(lexer, io_number));
		return (io_file(lexer, io_number));
	}
	if (lexer->curr_tok && (lexer->curr_tok->type == DLESS))
		return (io_here(lexer, NULL));
	return (io_file(lexer, NULL));
}
