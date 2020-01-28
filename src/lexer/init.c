/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _init.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:09:52 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/19 14:28:45 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			reset_lexer(t_lexer *lexer)
{
	static t_lexer	*lexer_ptr;

	if (lexer_ptr == NULL && lexer == NULL)
		return (12);
	if (lexer_ptr == NULL)
		lexer_ptr = lexer;
	if (lexer == NULL)
		lexer = lexer_ptr;
	lexer->state = START;
	free(lexer->str);
	token_del(&lexer->curr_tok);
	lexer->str = NULL;
	lexer->quote = 0;
	lexer->and_or = false;
	lexer->len = 0;
	lexer->i = 0;
	return (END_OF_INPUT);
}

int			init_lexer(t_lexer *lexer, t_input *input)
{
	reset_lexer(lexer);
	lexer->inputp = input;
	lexer->state = 0;
	lexer->state |= START;
	lexer->i = 0;
	return (0);
}
