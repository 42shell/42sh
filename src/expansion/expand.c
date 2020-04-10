/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/07 17:08:22 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/10 17:20:54 by fratajcz         ###   ########.fr       */
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

int		tilde_expand(t_dstr *str, char *home_dir)
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
*/

int		expand(t_simple_cmd *command, t_env *env)
{
	int		i;
	int		pos;
	char	*home_dir;
	t_token	*cur;

	i = 0;
	home_dir = get_env_var("HOME");
	cur = command->args;
	while (cur != NULL)
	{
		if (cur->type == WORD)
		{
			pos = tilde_expand(cur->value, home_dir);
			param_expand(cur->value, pos, env, false);
			path_expand(cur);
			remove_quotes(cur->value);
		}
		cur = cur->next;
	}
	return (0);
}
