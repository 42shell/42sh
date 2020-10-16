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

# define AMBIG_REDIR	2
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
	size_t len;

	if (home_dir == NULL)
		return (0);
	if (str->str[0] == '~'
	&& (str->str[1] == '\0' || str->str[1] == '/' || str->str[1] == ':'))
	{
		ft_dstr_remove(str, 0, 1);
		len = ft_strlen(home_dir);
		ft_dstr_insert(str, 0, home_dir, len);
		return (len);
	}
	return (0);
}

static int	expand_token(t_token *token, char *home_dir, bool split)
{
	int	pos;

	if (token->type == SPLIT_FIELD)
	{
		path_expand(token);
		return (0);
	}
	pos = tilde_expand(token->value, home_dir);
	token->exp_info = ft_dstr_dup(token->value);
	ft_memset(token->exp_info->str, '0', token->exp_info->len);
	if (dollar_expand(token, pos, false) == 1)
		return (1);
	if (split)
		split_fields(token);
	path_expand(token);
	remove_quotes(token);
	return (0);
}

static int	expand_token_list(t_token *token_list, char *home_dir, bool split)
{
	t_token	*cur;

	cur = token_list;
	while (cur != NULL)
	{
		if (cur->type == WORD || cur->type == SPLIT_FIELD)
			if (expand_token(cur, home_dir, split) == 1)
				return (1);
		cur = cur->next;
	}
	return (0);
}

int			expand_redir_list(t_redir *redir_list, char *home_dir)
{
	t_redir	*cur;
	t_redir *next;

	cur = redir_list;
	while (cur != NULL)
	{
		next = cur->next;
		if ((cur->left_op && cur->operator->type != DLESS
			&& expand_token(cur->left_op, home_dir, true) == 1)
		|| (cur->right_op && cur->operator->type != DLESS
			&& expand_token(cur->right_op, home_dir, true) == 1))
			return (1);
		cur = next;
	}
	cur = redir_list;
	while (cur)
	{
		if (cur->right_op && cur->right_op->next)
		{
			token_list_del(&cur->right_op->next);
			return (AMBIG_REDIR);
		}
		cur = cur->next;
	}
	return (0);
}

extern char *g_expand_error_token;

int			expand(t_simple_cmd *command)
{
	char	*home_dir;
	int		ret;

	home_dir = get_var_value("HOME");
	dup_command_args(command);
	if ((ret = expand_token_list(command->args_exp, home_dir, true))
	|| (ret = expand_token_list(command->assigns_exp, home_dir, false))
	|| (ret = expand_redir_list(command->redirs_exp, home_dir)))
	{
		if (ret == AMBIG_REDIR)
			ft_dprintf(2, "42sh: ambiguous redirect\n");
		return (1);
	}
	command->is_expand = true;
	return (0);
}
