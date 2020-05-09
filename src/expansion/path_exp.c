/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_exp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 14:14:32 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/24 01:49:04 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_array	*get_matches_in_dirs(t_array *directories, char *filename, bool no_dir)
{
	DIR				*dirp;
	struct dirent	*dp;
	size_t			i;
	t_array			*ret;

	i = 0;
	ret = array_new(0);
	while (i < directories->size)
	{
		if ((dirp = opendir(directories->array[i])))
		{
			while ((dp = readdir(dirp)))
			{
				if (is_match(dp->d_name, filename, NONE, true))
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
		directories = array_new(0);
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

void	add_matches_to_list(t_token *token, t_array *matches)
{
	char	**array;
	t_token	*cur;
	t_token *old_next;
	size_t	i;

	free(token->value->str);
	array = (char **)matches->array;
	token->value->str = ft_strdup(array[0]);
	token->value->len = ft_strlen(array[0]);
	token->value->size = token->value->len + 1;
	cur = token;
	old_next = token->next;
	i = 1;
	while (i < matches->size)
	{
		cur->next = token_new(WORD);
		ft_dstr_append(cur->next->value, array[i]);
		i++;
		cur = cur->next;
	}
	cur->next = old_next;
}

void	path_expand(t_token *token)
{
	t_array *matches;

	if (!has_glob_char(token->value->str))
		return ;
	matches = get_matches(token->value->str);
	if (matches->size == 0)
		return (array_destroy(matches));
	sort_matches((char **)(matches->array), matches->size);
	add_matches_to_list(token, matches);
	array_destroy(matches);
}
