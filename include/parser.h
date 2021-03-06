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

# define CMD_INVERT_RETURN	0x001
# define CMD_AMPERSAND		0x002
# define CMD_AND_IF			0x004
# define CMD_OR_IF			0x008
# define CMD_PIPE			0x010
# define CMD_COMPOUND		0x020
# define CMD_SUBSHELL		0x040
# define CMD_IF				0x080
# define CMD_ELIF			0x100
# define CMD_ELSE			0x200

enum						e_parser_status
{
	USER_ABORT = -1,
	NOERR = 0,
	UNEXPECTED_TOKEN,
	BRACKET_NEST_LIMIT_REACHED
};

enum						e_cmd_type
{
	AND_OR,
	PIPELINE,
	SIMPLE,
	GROUP,
	IF_CLAUSE,
	WHILE_CLAUSE
};

typedef struct				s_redir
{
	struct s_redir			*next;
	t_token					*left_op;
	t_token					*operator;
	t_token					*right_op;
	t_token					*heredoc;
}							t_redir;

typedef struct				s_simple_cmd
{
	t_redir					*redirs;
	t_redir					*redirs_exp;
	t_token					*args;
	t_token					*args_exp;
	t_token					*assigns;
	t_token					*assigns_exp;
	char					**argv;
	bool					is_expand;
}							t_simple_cmd;

typedef struct				s_if_clause
{
	struct s_command		*if_part;
	struct s_command		*then_part;
	struct s_command		*else_part;
}							t_if_clause;

union						u_cmd_value
{
	struct s_command		*and_or;
	struct s_command		*pipeline;
	struct s_command		*compound_list;
	struct s_if_clause		*if_clause;
	struct s_simple_cmd		*simple;
};

typedef struct				s_command
{
	enum e_cmd_type			type;
	int						flags;
	union u_cmd_value		value;
	struct s_command		*prev;
	struct s_command		*next;
	t_redir					*redir_list;
	t_redir					*redir_list_exp;
	int						sep;
}							t_command;

/*
** parser:
** -heredocs is a pointer to the right operand token of the
**  first io_here redir struct.
**  The tokens are chained if there are more than 1 heredocs,
**  and they are unchained in get_all_heredocs to avoid segfaults in
**  token_del()
*/

typedef struct				s_parser
{
	t_token					*token;
	t_token					*heredocs;
	enum e_parser_status	status;
	int						bracket_lvl;
}							t_parser;

extern t_parser				g_parser;

/*
** in case of line continuation
** -g_linebreak_type type determine the prompt displayed |, &&, ||, ( ...
** -when changing g_linebreak_type in a function the old linebreak type
**  is stored internally and reset after the line continuation
*/

extern int						g_linebreak_type;

t_command					*parse_complete_command(void);
t_command					*parse_and_or(void);
t_command					*parse_pipeline(void);
t_command					*parse_command(void);
t_command					*parse_simple_command(void);
t_command					*parse_compound_command(void);
t_command					*parse_brace_group(void);
t_command					*parse_subshell(void);

t_command					*parse_if_clause(void);
t_command					*parse_else_part(void);
t_command					*parse_if_then_statement(void);
t_command					*parse_while_clause(void);

t_command					*parse_do_group(void);
t_command					*parse_compound_list(void);
t_command					*parse_term(void);
t_redir						*parse_redirect_list(void);
t_redir						*parse_io_redirect(void);

int							parse_separator(void);
int							parse_separator_op(void);
int							parse_newline_list(void);
int							parse_linebreak(void);

int							get_required_reserv_word(int expect_type);

/*
** heredocs
*/

int							get_all_heredocs(void);
void						add_heredoc(t_token *heredoc);
int							handle_heredoc_eof(char *delim);

/*
** parse error
*/

t_command					*return_parse_error(t_command **to_del);
int							parse_error(char *near);

/*
** new/del
*/

t_command					*command_new(enum e_cmd_type type);
void						command_del(t_command **command);
void						redir_del(t_redir **redir);
void						complete_command_del(t_command
							**complete_command);

/*
** ast to str
*/

void						format_command(t_dstr *buf, t_command *command);
void						format_and_or(t_dstr *buf, t_command *command);
void						format_pipeline(t_dstr *buf, t_command *command);
void						format_group(t_dstr *buf, t_command *command);
void						format_if_clause(t_dstr *buf, t_command *command);
void						format_while_clause(t_dstr *buf,
												t_command *command);
void						format_compound_list(t_dstr *buf,
												t_command *command);
void						format_simple_command(t_dstr *buf,
													t_command *command);

/*
** notif
*/

void						print_command(t_command *command);

#endif
