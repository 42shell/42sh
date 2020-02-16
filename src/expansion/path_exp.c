/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_exp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 14:14:32 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/16 08:12:15 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "shell.h"

/*
** returns how much pat should advance if the bracket expression in pat matches,
** 0 if no match
** i == 1 exceptions are there because if ] is the first character, the ']' char
** must match 
*/

int		match_bracket(char c, char *pat)
{
	int		lowerc;
	int		upperc;	
	int		i;

	i = 1;
	while (pat[i] && (pat[i] != ']' || i == 1))
	{
		lowerc = pat[i];
		if (pat[i + 1] == '-')
		{
			i += 2;
			upperc = pat[i];
			if (c >= lowerc && c <= upperc)
				break ;
		}
		else if (lowerc == c)
			break ;
		i++;
	}
	if (pat[i] == ']' && i != 1)
		return (0);
	while (pat[i] && (pat[i] != ']' || i == 1))
		i++;
	return (pat[i] == '\0' ? 0 : i + 1);
}

bool	is_match(char *str, int i, char *pat, int j)
{
	static char quote;
	int			k;

	if (i == 0 && j == 0)
		quote = NONE;
	if (i == 0 && str[0] == '.' && quote == NONE
			&& (pat[j] == '*' || pat[j] == '?'))
		return (false);
	if (str[i] && (quote_start(str, i, &quote) || quote_stop(str, i, &quote)))
		return (is_match(str, i + 1, pat, j));
	if (pat[j] == '[' && quote == NONE && (k = match_bracket(str[i], &pat[j])))
		return (is_match(str, i + 1, pat, j + k));
	if (pat[j] == '*' && quote == NONE)
		return (str[i] == '\0'
			? is_match(str, i, pat, j + 1)
			: is_match(str, i, pat, j + 1)  || is_match(str, i + 1, pat, j));
	if (pat[j] == '?' && quote == NONE)
		return (str[i] != '\0' && is_match(str, i + 1, pat, j + 1));
	if (str[i] == '\0' && pat[j] == '\0')
		return (true);
	if (str[i] && str[i] == pat[j])
		return (is_match(str, i + 1, pat, j + 1));
	return (false);
}

bool	has_glob_char(char *str)
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
			if (str[i] == '[' && !bracket_open)
				bracket_open = true;
			if (str[i] == ']' && bracket_open)
				return (true);
		}
		quote_stop(str, i, &quote_status);
	}
	return (false);
}

t_array *get_matches_in_dirs(t_array *directories, char *filename, bool no_dir)
{
	DIR				*dirp;
	struct dirent	*dp;
	size_t			i;
	t_array			*ret;

	i = 0;
	ret = array_new();
	while (i < directories->size)
	{
		if ((dirp = opendir(directories->array[i])))
		{
			while ((dp = readdir(dirp)))
			{
				if (is_match(dp->d_name, 0, filename, 0))
					array_append(ret,
						no_dir ? ft_strdup(dp->d_name)
						: append_filename(directories->array[i], dp->d_name));
			}
			closedir(dirp);
		}
		i++;
	}
	array_destroy(directories);
	return (ret);
}

t_array	*get_dirs_to_search(char *dirname)
{
	t_array *directories;

	if (has_glob_char(dirname))
		directories = get_matches(dirname);
	else
	{
		directories = array_new();
		array_append(directories, ft_strdup(dirname));
	}
	return (directories);
}

t_array	*get_matches(char *path)
{
	char	*dirname;
	char	*filename;
	t_array *ret;
	bool	no_dir;
	int		i;

	no_dir = false;
	filename = ft_strrchr(path, '/');
	filename = (filename == NULL) ? path : filename + 1;
	i = filename - path;
	dirname = path;
	if (i > 0)
		dirname[i - 1] = '\0';
	else
	{
		dirname = ".";
		no_dir = true;
	}
	ret = get_matches_in_dirs(get_dirs_to_search(dirname), filename, no_dir);
	if (i > 0)
		path[i - 1] = '/';
	return (ret);
}


void	path_expand(t_node *pattern_node)
{
	t_array *matches;

	if (!has_glob_char(node_token(pattern_node)->value->str))
		return ;
	matches = get_matches(node_token(pattern_node)->value->str);
	if (matches->size == 0)
		return (array_destroy(matches));
	sort_matches((char **)(matches->array), matches->size);
	node_add_child(pattern_node, node_new(matches));
	node_token(pattern_node)->type = PATTERN;
}
