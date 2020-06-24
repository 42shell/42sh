/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 23:49:26 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 17:20:12 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
# include "shell.h"

# define BRACKET_NEST_LIMIT		1024

/*
** values for s_command->flags
*/

# define CMD_INVERT_RETURN		0x01
# define CMD_AMPERSAND			0x02
# define CMD_LASTPIPE			0x04
# define CMD_COMPOUND			0x08

enum							e_parser_status
{
	USER_ABORT = -1,
	NOERR = 0,
	UNEXPECTED_TOKEN,
	BRACKET_NEST_LIMIT_REACHED
};

enum							e_cmd_type
{
	CONNECTION,
	SIMPLE,
	COMPOUND,
	GROUP,
	IF_CLAUSE
};

typedef struct					s_redir
{
	struct s_redir				*next;
	t_token						*left_op;
	t_token						*operator;
	t_token						*right_op;
}								t_redir;

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
	bool						is_expand;
}								t_simple_cmd;

typedef struct					s_group_cmd
{
	struct s_command			*list;
	bool						subshell;
}								t_group_cmd;

typedef struct					s_if_clause
{
	struct s_command			*if_part;
	struct s_command			*then_part;
	struct s_command			*else_part;
}								t_if_clause;

union							u_cmd_value
{
	struct s_connection			*connection;
	struct s_simple_cmd			*simple;
	struct s_group_cmd			*group;
	struct s_if_clause			*if_clause;
};

typedef struct					s_command
{
	enum e_cmd_type				type;
	int							flags;
	union u_cmd_value			value;
	struct s_command			*next;
	t_redir						*redir_list;
	int							sep;
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
	int							bracket_lvl;
}								t_parser;

extern t_parser					g_parser;

/*
** in case of line continuation
** -g_linebreak_type type determine the prompt displayed |, &&, ||, ( ...
** -when changing g_linebreak_type in a function the old linebreak type
**  is stored internally and reset after the line continuation
*/

int								g_linebreak_type;

t_command						*parse_complete_command(void);
t_command						*parse_and_or(void);
t_command						*parse_pipeline(void);
t_command						*parse_command(void);
t_command						*parse_simple_command(void);
t_command						*parse_compound_command(void);
t_command						*parse_brace_group(void);
t_command						*parse_subshell(void);

t_command						*parse_if_clause(void);
t_command						*parse_else_part(void);

t_command						*parse_compound_list(void);
t_command						*parse_term(void);
t_redir							*parse_redirect_list(void);
t_redir							*parse_io_redirect(void);

int								parse_separator(void);
int								parse_separator_op(void);
int								parse_newline_list(void);
int								parse_linebreak(void);

int								get_all_heredocs(void);

int								parse_error(char *near);

/*
** utils
*/

void							add_heredoc(t_token *heredoc);
int								handle_heredoc_eof(char *delim);
int								get_required_reserv_word(int expect_type);

t_command						*command_new(enum e_cmd_type type);
int								command_del(t_command **command);
int								redir_del(t_redir **redir);
void							complete_command_del(t_command
								**complete_command);
void							print_command(t_command *command,
								int indent);

#endif
