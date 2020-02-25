/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 23:49:26 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/13 18:27:54 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "shell.h"

# define PARSE_ERROR	20

enum					e_parse_error
{
	SILENT_ABORT = -1,
	NOERR = 0,
	TOKENS_LEFT,
	NULL_AST,
	NULL_AST_NODE,
	NO_REDIR_FILENAME,
	NO_CMD_BEFORE_PIPE,
	NO_CMD_AFTER_PIPE,
	NO_CMD_BEFORE_AND_OR,
	NO_CMD_AFTER_AND_OR,
	HEREDOC_NO_DELIM,
	NO_CMD_BEFORE_SEP
};

typedef struct			s_ast
{
	t_node				*node;
	struct s_ast		*next;
	bool				run_in_background;
}						t_ast;

t_ast					*g_ast;

/*
** the current token being processed.
*/

t_token					*g_token;

/*
** g_heredocs' children are pointers to the heredoc nodes currently in the AST
*/

t_node					g_heredocs;

/*
** error
*/

int						g_parse_error;

int						parse(void);
t_node					*and_or(void);
t_node					*pipeline(void);
t_node					*io_redirect(void);

void					get_all_heredocs(t_node *heredoc_list);

void					free_ast_nodes(t_node *node, bool is_pattern);
t_token					*node_token(t_node *node);
void					print_ast(t_node *ast, int indent_level);

t_node					*parse_error(int code, char *near, t_node *to_free);

#endif
