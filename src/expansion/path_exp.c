/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_exp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 14:14:32 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/13 19:46:27 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

bool	is_match(char *str, char *pat, char quote)
{
	if (*str && (quote_start(str, 0, &quote) || quote_stop(str, 0, &quote)))
		return (is_match(str + 1, pat, quote));
	if (*pat == '*' && quote == NONE)
		return (*str == '\0'
			? is_match(str, pat + 1, quote)
			: is_match(str, pat + 1, quote) || is_match(str + 1, pat, quote));
	if (*str == '\0' && *pat == '\0')
		return (true);
	if (*str && *str == *pat)
		return (is_match(str + 1, pat + 1, quote));
	return (false);
}

bool	is_pattern(char *str)
{
	char	quote_status;
	bool	bracket_open;
	int		i;

	quote_status = NONE;
	i = -1;
	while (str[++i])
	{
		if (quote_start(str, i, &quote_status))
			continue ;
		if (quote_status == NONE)
		{
			if (str[i] == '*' || str[i] == '?')
				return (true);
			if (str[i] == '[')
				bracket_open = true;
			if (str[i] == ']' && bracket_open)
				return (true);
		}
		quote_stop(str, i, &quote_status);
	}
	return (false);
}

void	path_expand(t_node *pattern_node)
{
	char			*pattern;
	DIR				*dirp;
	struct dirent	*dp;

	if (!is_pattern(node_token(pattern_node)->value->str))
		return ;
	node_token(pattern_node)->type = PATTERN;
	pattern = ft_strdup(node_token(pattern_node)->value->str);
	if ((dirp = opendir(".")))
	{
		while ((dp = readdir(dirp)))
		{
			if (is_match(dp->d_name, pattern, NONE))				
				node_add_child(pattern_node, node_new(ft_strdup(dp->d_name)));
		}
		closedir(dirp);
	}
	sort_matches(pattern_node->child, pattern_node->nb_children);
	free(pattern);
}
