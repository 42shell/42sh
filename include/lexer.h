/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:09:52 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/13 18:01:19 by fratajcz         ###   ########.fr       */
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
	PATTERN
};

enum				e_quote_st
{
	NONE = 0,
	BSLASH = '\\',
	SQUOTE = '\'',
	DQUOTE = '"'
};

typedef struct		s_token
{
	t_dstr			*value;
	int				type;
}					t_token;

typedef struct		s_lexer
{
	t_token			*token;
	char			token_is_delim;
	char			line_cont;
	char			quote_st;
	size_t			i;
}					t_lexer;

t_lexer				g_lexer;

t_token				*get_next_token(void);

int					backslash_newline(void);
int					operator_next(void);
int					operator_end(void);
int					backslash(void);
int					quote(void);
int					operator_new(void);
int					blank(void);
int					word_next(void);
int					comment(void);
int					word_start(void);

t_token				*token_new(int type);
void				token_del(t_token **tok);

bool				is_operator_start(char c);
bool				is_operator_part(char c);
bool				is_operator_next(char *ope, char c);
bool				is_redir(t_token *token);
int					get_operator_type(char *ope);

#endif
