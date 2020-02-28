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
	g_parser.heredoc_ptr = &line[line_len];
	free(*heredoc);
	*heredoc = new;
}

static char	*get_heredoc(char *delim)
{
	int		ret;
	char	*heredoc;
	int		i;

	ret = 0;
	heredoc = NULL;
	if (g_parser.error)
		return (NULL);
	while (!line_eq(g_parser.heredoc_ptr, delim))
	{
		if (!*g_parser.heredoc_ptr)
		{
			i = g_parser.heredoc_ptr - g_lexer.line;
			if ((ret = get_input(PSH)) == RL_EOF || ret == RL_INT)
				break ;
			g_parser.heredoc_ptr = &g_lexer.line[i];
			continue ;
		}
		line_add(&heredoc, g_parser.heredoc_ptr);
	}
	if (ret != 0)
		ft_memdel((void **)&heredoc);
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
	|| !(g_parser.heredoc_ptr = g_lexer.line))
		return ;
	while ((g_parser.heredoc_ptr = ft_strchr(g_parser.heredoc_ptr, '\n') + 1)
	&& g_parser.heredocs[i])
	{
		ft_dstr_add(g_parser.heredocs[i]->value, '\n');
		if (!(heredoc_str = get_heredoc(g_parser.heredocs[i]->value->str)))
			break ;
		ft_dstr_del((void **)&g_parser.heredocs[i]->value);
		g_parser.heredocs[i]->value = ft_dstr_from_str(heredoc_str);
		free(heredoc_str);
		i++;
	}
	ft_memdel((void **)&g_parser.heredocs);
	g_parser.heredoc_ptr = NULL;
	free(heredoc_str);
}
