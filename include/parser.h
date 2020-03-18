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

enum					e_node_type
{
	NODE_PROCESS, //leaf
	NODE_PIPE,
	NODE_AND_IF,
	NODE_OR_IF
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
** job is complete_command():
** -root is the root of the tree (and_or())
** -next is the next complete_command
** -sep is the token used to separate the complete command, ';' or '&'
*/

typedef struct			s_job
{
	struct s_job		*next;
	struct s_job		*prev;
	t_node				*ast;
	t_token				*sep;
	pid_t				pgid;
	bool				notified;	/* true if user told about stopped job */
}						t_job;

/*
** process is simple command, it is a leaf of the tree:
** -argv is array of tokens (command name + args)
** -redirs struct has 3 token fields, from is NULL if no IO_NUMBER (set default value directly ??)
*/

typedef struct			s_redir
{
	t_token				*left_op;
	t_token				*operator;
	t_token				*right_op;
}						t_redir;

typedef struct			s_process
{
	char				**argv;
	t_redir				**redirs;
	char				*path;
	pid_t				pid;
	int					status;
	bool				stopped;
	char				done;
}						t_process;

/*
** parser:
** -heredocs are pointers to right operands of '<<' redir structs
*/

typedef struct			s_parser
{
	t_token				*token;
	t_token				**heredocs;
	int					error;
}						t_parser;

t_parser				g_parser;

t_job					*complete_command(void);
t_job					*list(void);
t_node					*and_or(t_node *left_pipeline);
t_node					*pipeline(void);
t_node					*simple_command(void);
t_redir					*io_redirect(void);
t_token					*separator(void);
t_token					*separator_op(void);
void					linebreak(void);
void					newline_list(void);

int						parse_error(int code, char *near);
void					get_all_heredocs(void);

t_job					*job_new();
int						job_del(t_job **jobs);
t_process				*process_new(void);
int						process_del(t_process **command);
t_redir					*redir_new(t_token *left_op, t_token *operator, t_token *right_op);
int						redir_del(t_redir **redir);

t_node					*node_new(void *data, int type);
int						node_add_child(t_node *parent, t_node *child);

void					free_ast_nodes(t_node *node, bool par_is_pattern);
void					print_ast(t_node *ast, size_t indent_level);

#endif
