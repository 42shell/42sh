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
	NOERR,
	TOKENS_LEFT,
	NULL_AST,
	NULL_AST_NODE,
	NO_REDIR_FILENAME,
	NO_CMD_BEFORE_PIPE,
	NO_CMD_AFTER_PIPE,
	HEREDOC_NO_DELIM,
	NO_CMD_BEFORE_SEP
};

typedef struct			s_node
{
	void				*data;
	int					nb_children;
	int					capacity;
	struct s_node		**child;
	int					type;
}						t_node;

/*
** ast is complete_command():
** -root is the root of the tree (and_or())
** -next is the next complete_command
** -sep is the token used to separate the complete command, ';' or '&'
*/

typedef struct			s_ast
{
	t_node				*root;
	struct s_ast		*next;
	t_token				*sep;
}						t_ast;

/*
** command is a leaf of the tree:
** -argv is array of tokens (command name + args)
** -redirs struct has 3 token fields, from is NULL if no IO_NUMBER (set default value directly ??)
*/

typedef struct			s_redir
{
	t_token				*left_op;
	t_token				*operator;
	t_token				*right_op;
}						t_redir;

typedef struct			s_command
{
	t_token				**argv;
	t_redir				**redirs;
}						t_command;

/*
** parser:
** -heredocs are pointers to right operands of '<<' redir structs
** -heredoc_ptr is a pointer to the line, it used by get_heredocs to
**  get the next lines of the heredoc
*/

typedef struct			s_parser
{
	t_token				*token;
	t_token				**heredocs;
	char				*heredoc_ptr;
	int					error;
}						t_parser;

t_parser				g_parser;

t_ast					*complete_command(void);
t_ast					*list(void);
t_node					*and_or(t_node *left_pipeline);
t_node					*pipeline(void);
t_node					*command(void);
t_redir					*io_redirect(void);
t_token					*separator(void);
t_token					*separator_op(void);
void					linebreak(void);
void					newline_list(void);

int						parse_error(int code, char *near);
void					get_all_heredocs(void);

t_ast					*ast_new();
int						ast_del(t_ast **ast);
t_command				*command_new(void);
int						command_del(t_command **command);
t_redir					*redir_new(t_token *left_op, t_token *operator, t_token *right_op);
int						redir_del(t_redir **redir);

t_node					*node_new(void *data, int type);
int						node_add_child(t_node *parent, t_node *child);
t_token					*node_token(t_node *node);

int						is_command(t_node *node);
void					free_ast_nodes(t_node *node, bool par_is_pattern);
void					print_ast(t_node *ast, size_t indent_level);

#endif
