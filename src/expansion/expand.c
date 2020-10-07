/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/07 17:08:22 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/17 04:56:02 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** see POSIX Shell Command Language 2.6 Word Expansions
**
**
** Returns the position in the string where we should start looking for param
** expansions because we don't want to expand any $ that was in $HOME.
** We don't check quote status in this function because the only case where a
** '~' should be expanded is if it's at the start of the word.
*/

int			tilde_expand(t_dstr *str, char *home_dir)
{
	char	*new;

	if (home_dir == NULL)
		return (0);
	if (str->str[0] == '~'
	&& (str->str[1] == '\0' || str->str[1] == '/' || str->str[1] == ':'))
	{
		new = ft_strjoin(home_dir, str->str + 1);
		free(str->str);
		str->str = new;
		return (ft_strlen(home_dir));
	}
	return (0);
}

/*
** If the complete expansion appropriate for a word results in an empty field,
** that empty field shall be deleted from the list of fields that form the
** completely expanded command, unless the original word contained single-quote
** or double-quote characters. -> bash does not seem to keep the empty field in
** any case ? would be easier to implement.
** ^ disregard that
** we store the next token before expanding because we may add new tokens after
** the token we expand, and we don't want to expand them
*/

static int	expand_token(t_token *token, char *home_dir)
{
	int	pos;

	pos = tilde_expand(token->value, home_dir);
	if (dollar_expand(token->value, pos, false) == 1)
		return (1);
	path_expand(token);
	remove_quotes(token->value);
	return (0);
}

static int	expand_token_list(t_token *token_list, char *home_dir)
{
	t_token	*cur;
	t_token *next;

	cur = token_list;
	while (cur != NULL)
	{
		next = cur->next;
		if (cur->type == WORD)
			if (expand_token(cur, home_dir) == 1)
				return (1);
		cur = next;
	}
	return (0);
}

static int	expand_redir_list(t_redir *redir_list, char *home_dir)
{
	t_redir	*cur;
	t_redir *next;

	cur = redir_list;
	while (cur != NULL)
	{
		next = cur->next;
		if (cur->left_op && cur->operator->type != DLESS)
			if (expand_token(cur->left_op, home_dir) == 1)
				return (1);
		if (cur->right_op && cur->operator->type != DLESS)
			if (expand_token(cur->right_op, home_dir) == 1)
				return (1);
		cur = next;
	}
	return (0);
}

extern char *g_expand_error_token;

int			expand(t_simple_cmd *command)
{
	char	*home_dir;
	t_redir	*cur;

	home_dir = get_var_value("HOME");
	dup_command_args(command);
	if (expand_token_list(command->args_exp, home_dir) == 1
	|| expand_token_list(command->assigns_exp, home_dir) == 1
	|| expand_redir_list(command->redirs_exp, home_dir) == 1)
		return (1);
	cur = command->redirs_exp;
	while (cur)
	{
		if (cur->right_op && cur->right_op->next)
		{
			token_list_del(&cur->right_op->next);
			ft_dprintf(2, "42sh: ambiguous redirect\n");
			return (1);
		}
		cur = cur->next;
	}
	command->is_expand = true;
	return (0);
}
