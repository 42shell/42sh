/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   param_exp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 16:36:33 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/17 04:57:14 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** replace a substring of a token between start and end (inclusive)
** (token "0123456789", 3, 6, "test") -> 012test789
*/

void		token_replace_between(t_token *token, int start, int end,
									char *replacement)
{
	size_t	len_to_delete;
	size_t	repl_len;

	len_to_delete = end - start + 1;
	repl_len = ft_strlen(replacement);
	ft_dstr_remove(token->value, start, len_to_delete);
	ft_dstr_insert(token->value, start, replacement, repl_len);
	if (token->exp_info)
	{
		ft_dstr_remove(token->exp_info, start, len_to_delete);
		ft_dstr_insert(token->exp_info, start, replacement, repl_len);
		ft_memset(token->exp_info->str + start, '1', repl_len);
	}
}

void		token_replace_from_i(t_token *token, int *i, char *new_str
								, int erase_past_end)
{
	size_t	to_replace_len;
	size_t	new_len;

	to_replace_len = ft_strlen(token->value->str + *i) + erase_past_end;
	new_len = ft_strlen(new_str);
	ft_dstr_remove(token->value, *i, to_replace_len);
	ft_dstr_insert(token->value, *i, new_str, new_len);
	if (token->exp_info)
	{
		ft_dstr_remove(token->exp_info, *i, to_replace_len);
		ft_dstr_insert(token->exp_info, *i, new_str, new_len);
		ft_memset(token->exp_info->str + *i, '1', new_len);
	}
	*i += new_len - 1;
}

/*
** Returns -1 if var name is invalid or braces aren't closed
** start is the index of the leading '$' char
*/

static int	get_var_end(char *str, int start, bool brace)
{
	int	i;
	int	limit;

	limit = INT_MAX;
	i = start + 1;
	if (brace)
	{
		if ((limit = get_end_of_braces(str, i - 1)) == -1)
			return (-1);
		i = start + 2;
	}
	if (str[i] == '$' || str[i] == '!' || str[i] == '?')
	{
		if (!brace || str[i + 1] == '}')
			return (i + 1);
	}
	if (ft_isdigit(str[i]))
		return (-1);
	while ((ft_isalnum(str[i]) || str[i] == '_') && i < limit - 1)
		i++;
	if (i == start + 1 || (brace && (i != limit - 1 || !ft_isalnum(str[i]))))
		return (-1);
	return (i + (brace ? 1 : 0));
}

/*
** get_var_end returns the index past the variable name
** e.g for $HOME,lol it returns the index of ',' which is why when replacing
** by the var name, if there is no brace we replace between start and end - 1
*/

int			param_expand(t_token *token, int *i, bool brace)
{
	int		end;
	char	oldchar;
	char	*var_value;

	end = get_var_end(token->value->str, *i, brace);
	if (end == -1)
	{
		if (brace)
		{
			ft_dprintf(2, "42sh: %s: bad substitution\n",
					token->value->str + *i);
			return (1);
		}
		return (0);
	}
	oldchar = token->value->str[end];
	token->value->str[end] = '\0';
	var_value = get_var_value(token->value->str + *i + 1 + brace);
	token->value->str[end] = oldchar;
	token_replace_between(token, *i, end - !brace, var_value);
	*i += ft_strlen(var_value) - 1;
	return (0);
}
