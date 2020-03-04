/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/07 20:03:18 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/07 02:50:32 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static char	*g_heredoc_ptr = NULL;

static bool	line_eq(char *s1, char *s2)
{
	if (!s1 || !s2)
		return (false);
	while (*s1 && *s2 && *s1 != '\n' && *s2 != '\n')
	{
		if (*s1++ != *s2++)
			return (false); 
	}
	return (*s1 == *s2);
}

static void	line_add(char **heredoc, char *line)
{
	int		heredoc_len;
	int		line_len;
	char	*new;

	line_len = 0;
	heredoc_len = 0;
	if (!heredoc || !line)
		return ;
	while (line[line_len] && line[line_len++] != '\n')
		;
	if (*heredoc)
		heredoc_len = ft_strlen(*heredoc);
	new = ft_xmalloc(heredoc_len + line_len + 1);
	ft_memcpy(new, *heredoc, heredoc_len);
	ft_memcpy(&new[heredoc_len], line, line_len);
	g_heredoc_ptr = &line[line_len];
	g_lexer.i += line_len;
	free(*heredoc);
	*heredoc = new;
}

static int	line_get(char **heredoc, char *delim)
{
	int		ret;

	ret = 0;
	delim[ft_strlen(delim) - 1] = 0;
	if ((ret = g_shell.get_input(PSH)) != 0)
	{
		if (ret == RL_INT)
			ft_memdel((void **)heredoc);
		else
		{
			parse_error(HEREDOC_NO_DELIM, delim);
			delim = NULL;
		}
	}
	free(delim);
	return (ret);
}

static char	*get_heredoc(char *delim)
{
	char	*heredoc;
	int		i;

	heredoc = NULL;
	if (g_parser.error)
		return (NULL);
	while (!line_eq(g_heredoc_ptr, delim))
	{
		if (!*g_heredoc_ptr)
		{
			i = g_heredoc_ptr - g_lexer.line;
			while (!ft_strchr(g_heredoc_ptr, '\n'))
			{
				if (line_get(&heredoc, ft_strdup(delim)) != 0)
					return (heredoc);
				g_heredoc_ptr = &g_lexer.line[i];
			}
			continue ;
		}
		line_add(&heredoc, g_heredoc_ptr);
	}
	g_lexer.i += ft_strlen(delim);
	return (heredoc);
}

/*
** dstr may be more efficient
*/

void		get_all_heredocs(void)
{
	char	*heredoc_str;
	int		i;

	i = 0;
	if (g_parser.error || !g_parser.heredocs
	|| !(g_heredoc_ptr = g_lexer.line))
		return ;
	while ((g_heredoc_ptr = ft_strchr(g_heredoc_ptr, '\n') + 1)
	&& g_parser.heredocs[i])
	{
		ft_dstr_add(g_parser.heredocs[i]->value, '\n');
		if (!(heredoc_str = get_heredoc(g_parser.heredocs[i]->value->str)))
			break ;
		ft_dstr_del((void **)&g_parser.heredocs[i]->value);
		g_parser.heredocs[i]->value = ft_dstr_from_str(heredoc_str);
		ft_memdel((void **)&heredoc_str);
		i++;
	}
	ft_memdel((void **)&heredoc_str);
	g_heredoc_ptr = NULL;
}
