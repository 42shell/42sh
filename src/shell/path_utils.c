/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 10:28:53 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/22 14:48:14 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

extern int	g_last_exit_st;

static int	get_nb_words(char const *s)
{
	int word_count;

	word_count = 1;
	while (*s)
	{
		if (*s == ':')
			word_count++;
		s++;
	}
	return (word_count);
}

static int	get_wlen(char const *s)
{
	int i;

	i = 0;
	while (s[i] && s[i] != ':')
		i++;
	return (i);
}

char		**split_path(char const *path)
{
	char	**arr;
	int		i;
	int		nb_words;
	int		wlen;

	if (path == NULL)
		return (NULL);
	nb_words = get_nb_words(path);
	if (!(arr = ft_xmalloc(sizeof(char *) * (nb_words + 1))))
		return (NULL);
	i = 0;
	while (i < nb_words)
	{
		if (*path == ':' && i != 0)
			path++;
		wlen = get_wlen(path);
		arr[i] = (wlen == 0) ? ft_strdup(".") : ft_strndup(path, wlen);
		if (arr[i] == NULL)
			return (NULL);
		path += wlen;
		i++;
	}
	arr[i] = 0;
	return (arr);
}

char		*append_filename(char *path, char *filename)
{
	char	*ret;
	int		len_path;
	int		i;

	len_path = ft_strlen(path);
	ret = ft_xmalloc(len_path + ft_strlen(filename) + 2);
	i = 0;
	while (path[i])
	{
		ret[i] = path[i];
		i++;
	}
	if (ret[i - 1] != '/')
		ret[i++] = '/';
	while (*filename)
	{
		ret[i] = *filename;
		i++;
		filename++;
	}
	ret[i] = '\0';
	return (ret);
}

char		*get_executable_path(char *command, t_env *env)
{
	char		**path;
	char		*ret;
	int			i;
	struct stat b;

	if (ft_strchr(command, '/'))
		return (ft_strdup(command));
	ret = get_env_var("PATH", env);
	path = split_path(ret);
	i = -1;
	while (path && path[++i] && (ret = append_filename(path[i], command)))
	{
		if (stat(ret, &b) == 0 && S_IXUSR & b.st_mode && !S_ISDIR(b.st_mode))
			break ;
		free(ret);
	}
	if ((path == NULL || path[i] == NULL))
	{
		ft_dprintf(2, "21sh: %s: command not found\n", command);
		fflush(stdout);
		g_last_exit_st = 127;
		ret = NULL;
	}
	free_arr(path);
	return (ret);
}
