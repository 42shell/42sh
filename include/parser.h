/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 23:49:26 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/10 14:45:40 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "shell.h"

enum							e_parser_status
{
	USER_ABORT = -1,
	NOERR = 0,
	UNEXPECTED_TOKEN
};

/*
** -A single redir is returned by parse_io_redirect()
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

enum							e_cmd_type
{
	CONNECTION,
	SIMPLE,
	SUBSHELL
};

typedef struct					s_connection
{
	struct s_command			*left;
	struct s_command			*right;
	enum e_token_type			connector;
}								t_connection;

typedef struct					s_simple_cmd
{
	t_redir						*redirs;
	t_token						*args;
	t_token						*assigns;
	char						**argv;
}								t_simple_cmd;

/*
** maybe useless
*/

typedef struct					s_subshell
{
	int							flags;
	struct s_command			*command;
}								t_subshell;

union							u_cmd_value
{
	struct s_connection			*connection;
	struct s_simple_cmd			*simple;
	struct s_subshell			*subshell;
};


/*
** values for s_command->flags
*/

# define CMD_INVERT_RETURN		0x01
# define CMD_AMPERSAND			0x02
# define CMD_LASTPIPE			0x04

typedef struct					s_command
{
	enum e_cmd_type				type;
	int							flags;
	union u_cmd_value			value;
	struct s_command			*next;
}								t_command;

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
	enum e_parser_status		status;
}								t_parser;

t_parser						g_parser;

t_command						*parse_and_or(void);
t_command						*parse_pipeline(void);
t_command						*parse_pipe_sequence(void);
t_command						*parse_command(void);
t_command						*parse_simple_command(void);
t_command						*parse_command_list(void);
t_redir							*parse_io_redirect(void);
int								parse_separator(void);
bool							parse_newline_list(void);
void							parse_linebreak(int last_token_type);
int								get_all_heredocs(void);

int								parse_error(char *near);
int								parse_heredoc_eof(char *delim);

t_command						*command_new(enum e_cmd_type type);
int								command_del(t_command **command);
int								redir_del(t_redir **redir);
void							command_list_del(t_command **command_list);

#endif
