/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_exp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 14:14:32 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/13 21:11:53 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

bool	is_match(char *str, int i, char *pat, int j)
{
	static char quote;
	if (i == 0)
		quote = NONE;
	if (str[i] && (quote_start(str, i, &quote) || quote_stop(str, i, &quote)))
		return (is_match(str, i + 1, pat, j));
	if (pat[j] == '*' && quote == NONE)
		return (str[i] == '\0'
			? is_match(str, i, pat, j + 1)
			: is_match(str, i, pat, j + 1)  || is_match(str, i + 1, pat, j));
	if (str[i] == '\0' && pat[j] == '\0')
		return (true);
	if (str[i] && str[i] == pat[j])
		return (is_match(str, i + 1, pat, j + 1));
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
			if (is_match(dp->d_name, 0, pattern, 0))				
				node_add_child(pattern_node, node_new(ft_strdup(dp->d_name)));
		}
		closedir(dirp);
	}
	sort_matches(pattern_node->child, pattern_node->nb_children);
	free(pattern);
}
