/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/10 21:35:08 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/10 21:35:10 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arith.h"

t_ht		*g_operator_types;
t_lexer		g_arith_lexer;

t_token						*get_id_token(void)
{
	char	c;
	t_token	*token;

	token = token_new(ID);
	while (ft_isalnum((c = g_arith_lexer.line[g_arith_lexer.i])) || c == '_')
	{
		ft_dstr_add(token->value, c);
		g_arith_lexer.i++;
	}
	return (token);
}

t_token						*get_num_token(void)
{
	char	c;
	t_token	*token;

	token = token_new(NUM);
	while (ft_isalnum((c = g_arith_lexer.line[g_arith_lexer.i])))
	{
		ft_dstr_add(token->value, c);
		g_arith_lexer.i++;
	}
	return (token);
}

t_token						*get_operator_token(void)
{
	t_token	*token;
	int		*type;

	token = token_new(-1);
	ft_dstr_add(token->value, g_arith_lexer.line[g_arith_lexer.i]);
	ft_dstr_add(token->value, g_arith_lexer.line[g_arith_lexer.i + 1]);
	type = ht_get(g_operator_types, token->value->str);
	if (type != NULL)
	{
		g_arith_lexer.i += 2;
		token->type = *type;
		return (token);
	}
	ft_dstr_remove(token->value, 1, 1);
	type = ht_get(g_operator_types, token->value->str);
	if (type != NULL)
	{
		g_arith_lexer.i += 1;
		token->type = *type;
		return (token);
	}
	token_del(&token);
	return (NULL);
}

t_token						*get_token_list(const char *line)
{
	char	c;

	init_lexer(line);
	while ((c = g_arith_lexer.line[g_arith_lexer.i]) != '\0')
	{
		if (c == ' ' || c == '\n')
			g_arith_lexer.i++;
		else
		{
			if (ft_isalpha(c))
				g_arith_lexer.cur_tok->next = get_id_token();
			else if (ft_isdigit(c))
				g_arith_lexer.cur_tok->next = get_num_token();
			else if (!(g_arith_lexer.cur_tok->next = get_operator_token()))
			{
				token_list_del(&g_arith_lexer.token_list);
				return (NULL);
			}
			g_arith_lexer.cur_tok = g_arith_lexer.cur_tok->next;
		}
	}
	g_arith_lexer.cur_tok = g_arith_lexer.token_list->next;
	token_del(&g_arith_lexer.token_list);
	return (g_arith_lexer.cur_tok);
}
