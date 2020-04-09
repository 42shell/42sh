/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:09:52 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/09 18:15:44 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "shell.h"

/*
** -----------------------
**   The grammar symbols
** -----------------------
**
** %token  DLESS  DGREAT  LESSAND  GREATAND  LESSGREAT  DLESSDASH  AMPERSAND
**         '<<'   '>>'    '<&'     '>&'      '<>'         <<-        '&'
**		  SEMI    PIPE    LESS   GREAT
**		   ';'	   '|'     '<'    '>'
*/

enum				e_token_type
{
	END_OF_INPUT,
	NEWLINE,
	WORD,
	ASSIGNMENT_WORD,
	NAME,
	IO_NUMBER,
	LESS,
	GREAT,
	DLESS,
	DGREAT,
	LESSAND,
	GREATAND,
	AMPERSAND,
	SEMI,
	PIPE,
	AND_IF,
	OR_IF,
	BANG,
	PATTERN
};

enum				e_quote_st
{
	NONE = 0,
	BSLASH = '\\',
	SQUOTE = '\'',
	DQUOTE = '"'
};

/*
** -token_del frees the entire list of tokens
** -the lexer do not chain them.
*/

typedef struct		s_token
{
	struct s_token		*next;
	t_dstr				*value;
	enum e_token_type	type;
}					t_token;

typedef struct		s_lexer
{
	char			*line;
	t_token			*token;
	char			token_delimited;
	char			line_cont;
	char			quote_st;
	char			nl_found;
	char			end_of_input;
	size_t			i;
}					t_lexer;

t_lexer				g_lexer;

t_token				*get_next_token(void);
int					reset_lexer(void);

int					lx_end(void);
int					lx_operator_next(void);
int					lx_operator_end(void);
int					lx_backslash(void);
int					lx_quote(void);
int					lx_operator_new(void);
int					lx_newline(void);
int					lx_blank(void);
int					lx_word_next(void);
int					lx_comment(void);
int					lx_word_start(void);

t_token				*token_new(int type);
void				token_del(t_token **tok);

bool				is_operator_start(char c);
bool				is_operator_part(char c);
bool				is_operator_next(char *ope, char c);
bool				is_redir(t_token *token);
int					get_operator_type(char *ope);

#endif
