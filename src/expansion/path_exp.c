/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_exp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 14:14:32 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/14 16:14:05 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "shell.h"

bool	is_match(char *str, int i, char *pat, int j)
{
	static char quote;
	if (i == 0 && j == 0)
		quote = NONE;
	if (str[0] == '.' && quote == NONE && pat[j] == '*')
		return (false);
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
			if (str[i] == '[')
				bracket_open = true;
			if (str[i] == ']' && bracket_open)
				return (true);
		}
		quote_stop(str, i, &quote_status);
	}
	return (false);
}

char	**get_matches_in_dirs(char **directories, char *filename, bool no_dir)
{
	DIR				*dirp;
	struct dirent	*dp;
	int				j;
	int				k;
	char			**ret;

	ret = malloc(sizeof(char *) * 512);
	j = 0;
	k = -1;
	while (directories[++k])
	{
		if ((dirp = opendir(directories[k])))
		{
			while ((dp = readdir(dirp)))
			{
				if (is_match(dp->d_name, 0, filename, 0))
					ret[j++] = no_dir ? ft_strdup(dp->d_name)
								: append_filename(directories[k], dp->d_name);
			}
			closedir(dirp);
		}
	}
	ret[j] = NULL;
	free_arr(directories);
	return (ret);
}

char	**get_dirs_to_search(char *dirname)
{
	char	**directories;

	if (has_glob_char(dirname))
		directories = get_matches(dirname);
	else
	{
		directories = malloc(sizeof(char *) * 2);
		directories[0] = ft_strdup(dirname);
		directories[1] = NULL;
	}
	return (directories);
}

char	**get_matches(char *path)
{
	char	*dirname;
	char	*filename;
	char	**ret;
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
	int		i;
	char	**res;

	if (!has_glob_char(node_token(pattern_node)->value->str))
		return ;
	res = get_matches(node_token(pattern_node)->value->str);
	i = 0;
	free_arr(res);
	//sort_matches(pattern_node->child, pattern_node->nb_children);
}
