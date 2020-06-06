/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_lexer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/06 18:57:44 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/06 19:34:28 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arith.h"

static enum e_token_type	*malloc_enum(enum e_token_type type)
{
	enum e_token_type *ptr;

	ptr = ft_xmalloc(sizeof(enum e_token_type));
	*ptr = type;
	return (ptr);
}

static void					init_operator_types_ht2(void)
{
	ht_put(g_operator_types, "-", malloc_enum(MINUS));
	ht_put(g_operator_types, "+", malloc_enum(PLUS));
	ht_put(g_operator_types, "*", malloc_enum(MUL));
	ht_put(g_operator_types, "/", malloc_enum(DIV));
	ht_put(g_operator_types, "%", malloc_enum(MOD));
	ht_put(g_operator_types, "?", malloc_enum(QMARK));
	ht_put(g_operator_types, ":", malloc_enum(COLON));
	ht_put(g_operator_types, "(", malloc_enum(LPAREN));
	ht_put(g_operator_types, ")", malloc_enum(RPAREN));
}

void						init_operator_types_ht(void)
{
	g_operator_types = ht_new(32, free);
	ht_put(g_operator_types, "=", malloc_enum(ASSIGN));
	ht_put(g_operator_types, "+=", malloc_enum(ADD_ASIGN));
	ht_put(g_operator_types, "-=", malloc_enum(SUB_ASSIGN));
	ht_put(g_operator_types, "*=", malloc_enum(MUL_ASSIGN));
	ht_put(g_operator_types, "/=", malloc_enum(DIV_ASSIGN));
	ht_put(g_operator_types, "++", malloc_enum(PLUSPLUS));
	ht_put(g_operator_types, "--", malloc_enum(MINMIN));
	ht_put(g_operator_types, "||", malloc_enum(OR));
	ht_put(g_operator_types, "&&", malloc_enum(AND));
	ht_put(g_operator_types, "!", malloc_enum(NOT));
	ht_put(g_operator_types, "<=", malloc_enum(LESS_EQUAL));
	ht_put(g_operator_types, "<", malloc_enum(LESS));
	ht_put(g_operator_types, ">", malloc_enum(MORE));
	ht_put(g_operator_types, ">=", malloc_enum(MORE_EQUAL));
	ht_put(g_operator_types, "==", malloc_enum(EQUALEQUAL));
	ht_put(g_operator_types, "!=", malloc_enum(NOT_EQUAL));
	init_operator_types_ht2();
}

void						init_lexer(const char *str)
{
	if (g_operator_types == NULL)
		init_operator_types_ht();
	g_arith_lexer.i = 0;
	g_arith_lexer.line = str;
	g_arith_lexer.token_list = token_new(-1);
	g_arith_lexer.cur_tok = g_arith_lexer.token_list;
}
