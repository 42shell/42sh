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
	HEREDOC_NO_DELIM,
	NO_CMD_BEFORE_SEP
};

/*
** job struct is and_or()
** -ast is the root of the tree
** -next is the next job
** -heredocs are pointers to the heredoc nodes currently in the AST
** -bg is background
*/

typedef struct			s_job
{
	t_node				*ast;
	struct s_job		*next;
	bool				bg;
	/*
	** job stuff
	*/
}						t_job;

/*
** process struct is command()
** -argv is array of tokens (command name + args)
** -redirs struct has 3 token fields, from is NULL if no IO_NUMBER
*/

typedef struct			s_redir
{
	t_token				*from;
	t_token				*redir_op;
	t_token				*to;
	//t_dstr			**heredoc; || global
}						t_redir;

typedef struct			s_process
{
	t_token				**argv;
	t_redir				**redirs;
	/*
	** process stuff;
	*/
}						t_process;

/*
** the current token being processed.
*/

t_token					*g_token;

/*
** error
*/

int						g_parse_error;

t_job					*get_job(void);
t_node					*and_or(void);
t_node					*pipeline(void);
t_redir					*io_redirect(void);
void					del_job(t_job **job);
void					*parse_error(int code, char *near, void *to_free);

void					get_all_heredocs(t_node *heredoc_list);

t_process				*process_new(void);
void					process_del(t_process **process);
void					redir_del(t_redir **redir);
int						is_process(t_node *node);

t_token					*node_token(t_node *node);
void					free_ast_nodes(t_node *node, bool par_is_pattern);
void					print_ast(t_node *ast, size_t indent_level);

#endif
