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

typedef struct			s_ast
{
	t_node				*node;
	struct s_ast		*next;
	bool				run_in_background;
}						t_ast;

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
	HEREDOC_NO_DELIM,
	NO_CMD_BEFORE_SEP
};

int						parse(t_lexer *lexer, t_env *env, t_term *term);
void					print_ast(t_node *ast, int indent_level);
t_node					*io_redirect(t_lexer *lexer);
t_node					*pipeline(t_lexer *lexer);
void					free_ast_nodes(t_node *node, bool is_pattern);
void					get_all_heredocs(t_input *input,
						t_node *heredoc_list);

t_token					*node_token(t_node *node);

#endif
