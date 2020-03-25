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

# define PARSE_ERROR			20 //?

enum							e_parse_error
{
	SILENT_ABORT = -1,
	NOERR,
	TOKENS_LEFT,
	NULL_AST,
	NULL_AST_NODE,
	NO_REDIR_FILENAME,
	NO_CMD_BEFORE_OP,
	NO_CMD_AFTER_OP,
	HEREDOC_NO_DELIM
};

enum							e_node_type
{
	NODE_SIMPLE_COMMAND,
	NODE_COMMAND,
	NODE_PIPELINE,
	NODE_BANG,
	NODE_AND_IF,
	NODE_OR_IF,
	NODE_SEP,
};

typedef struct					s_node
{
	struct s_node				*left;
	struct s_node				*right;
	void						*data; //just remove this and put everything in nodes
	int							type;
}								t_node;

/*
** -A single redir is returned by ps_io_redirect()
** -it contains 3 tokens corresponding to the operator and
**  the 2 operands of a redirection (2 >& 1)
** -the left operand can be NULL
*/

typedef struct					s_redir
{
	struct s_redir				*next;
	t_token						*left_op;
	t_token						*operator;
	t_token						*right_op;
}								t_redir;

typedef struct					s_command
{
	t_token						*words;
	t_redir						*redirs;
}								t_command;

typedef struct					s_list//remove list and command
{
	struct s_list				*next;
	t_node						*ast;
	int							bg;
}								t_list;

/*
** parser:
** -heredocs is a pointer to the right operand token of the
**  first io_here redir struct.
**  The tokens are chained if there are more than 1 heredocs,
**  and they are unchained in get_all_heredocs to avoid segfaults in
**  token_del()
*/

typedef struct					s_parser
{
	t_token						*token;
	t_token						*heredocs;
	int							error;
}								t_parser;

t_parser						g_parser;

t_list							*ps_list(void); //node *
t_node							*ps_and_or(void);
t_node							*ps_pipeline(void);
t_node							*ps_pipe_sequence(void);
t_node							*ps_command(void);
t_node							*ps_simple_command(void);
t_redir							*ps_io_redirect(void);
t_token							*ps_separator(void);
t_token							*ps_separator_op(void);
void							ps_newline_list(void);
void							ps_linebreak(int last_token_type);
int								ps_get_all_heredocs(void);

int								ps_error(char *near);
int								ps_heredoc_eof(char *delim);

int								list_del(t_list **list);
int								ast_del(t_node **ast);
int								command_del(t_command **command);
int								redir_del(t_redir **redir);

void							print_ast(t_node *ast, int indent);

#endif
