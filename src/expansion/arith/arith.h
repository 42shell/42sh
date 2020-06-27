/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arith.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/06 16:26:07 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/11 01:46:36 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARITH_H
# define ARITH_H

# include "libft.h"
# include <termios.h>

typedef struct s_job	t_job;
typedef int				(*t_input_func)(const char *, bool);

typedef struct	s_shell
{
	bool			interactive_mode;
	t_input_func	get_input;
	pid_t			pgid;
	struct termios	tmodes;
	t_ht			*vars;
}				t_shell;

typedef char *(*t_var_value_func)(void);

typedef struct	s_var
{
	char				*name;
	char				*value;
	char				*exportstr;
	int					attributes;
	t_var_value_func	value_func;
}				t_var;

void			set_var(const char *name, const char *value,
						const int attributes);

extern t_shell	g_shell;

/*
** LEXER
*/

enum			e_token_type
{
	ID,
	NUM,
	ASSIGN,
	ADD_ASIGN,
	SUB_ASSIGN,
	MUL_ASSIGN,
	DIV_ASSIGN,
	PLUSPLUS,
	MINMIN,
	OR,
	AND,
	NOT,
	LESS_EQUAL,
	LESS,
	MORE,
	MORE_EQUAL,
	EQUALEQUAL,
	NOT_EQUAL,
	MINUS,
	PLUS,
	MUL,
	DIV,
	MOD,
	QMARK,
	COLON,
	LPAREN,
	RPAREN,
	NB_OP
};

typedef struct	s_token
{
	struct s_token		*next;
	t_dstr				*value;
	t_dstr				*exp_info;
	enum e_token_type	type;
}				t_token;

typedef struct	s_lexer
{
	int				i;
	const char		*line;
	t_token			*token_list;
	t_token			*cur_tok;
}				t_lexer;

extern t_ht		*g_operator_types;
extern t_lexer	g_arith_lexer;

t_token			*token_new(int type);
void			token_del(t_token **tok);
void			token_list_del(t_token **token);

void			init_lexer(const char *str);
t_token			*get_token_list(const char *line);

/*
** PARSER
*/

enum			e_parser_status
{
	OK,
	ERR = 1
};

extern enum e_parser_status g_arith_status;
extern t_token				*g_cur_tok;
extern int					g_paren_lvl;

typedef struct	s_node
{
	struct s_node	*left;
	struct s_node	*right;
	t_token			*token;
}				t_node;

void			*arith_parse_error(void);
t_node			*node_new(t_token *token);
void			node_del(t_node **node);
void			discard_token(void);

bool			is_rel_op(enum e_token_type type);
bool			is_assignment(enum e_token_type type);

t_node			*parse(t_token *token_list);

t_node			*parse_mutable(void);
t_node			*parse_factor(void);
t_node			*parse_unary_rel_expression(void);
t_node			*parse_expression();

/*
** EVAL
*/

long			eval_minus_node(t_node *node);
long			eval_plus_node(t_node *node);
long			eval_mul_node(t_node *node);
long			eval_div_node(t_node *node);
long			eval_mod_node(t_node *node);
long			eval_num_node(t_node *node);
long			eval_or_node(t_node *node);
long			eval_and_node(t_node *node);
long			eval_not_node(t_node *node);
long			eval_less_equal_node(t_node *node);
long			eval_less_node(t_node *node);
long			eval_more_node(t_node *node);
long			eval_more_equal_node(t_node *node);
long			eval_equalequal_node(t_node *node);
long			eval_not_equal_node(t_node *node);
long			eval_id_node(t_node *node);
long			eval_assign_node(t_node *node);
long			eval_add_assign_node(t_node *node);
long			eval_sub_assign_node(t_node *node);
long			eval_mul_assign_node(t_node *node);
long			eval_div_assign_node(t_node *node);
long			eval_plusplus_node(t_node *node);
long			eval_minusminus_node(t_node *node);
long			eval_arith_ast(t_node *ast);
long			eval_expr(const char *str);

#endif
