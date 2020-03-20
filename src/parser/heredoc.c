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
char		g_heredoc_eof = false;

static bool		line_eq(char *s1, char *s2)
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

static void		line_add(t_dstr *heredoc)
{
	int		line_len;

	line_len = 0;
	while (g_heredoc_ptr[line_len])
	{
		if (g_heredoc_ptr[line_len++] == '\n')
			break ;
	}
	ft_dstr_insert(heredoc, heredoc->len, g_heredoc_ptr, line_len);
	g_heredoc_ptr += line_len;
}

static int		line_get(void)
{
	int		ret;
	int		i;

	ret = 0;
	i = g_heredoc_ptr - g_lexer.line;
	while (!ft_strchr(g_heredoc_ptr, '\n'))
	{
		i = g_heredoc_ptr - g_lexer.line;
		ret = g_shell.get_input(PSH);
		g_heredoc_ptr = &g_lexer.line[i];
		if (ret != 0)
		{
			if (ret == INPUT_EOF)
				g_parser.error = NOERR;
			break ;
		}
	}
	return (ret);
}

static t_dstr	*get_heredoc(char *delim)
{
	t_dstr	*heredoc;
	int		ret;

	heredoc = ft_dstr_new(32);
	while (!g_heredoc_eof && !line_eq(g_heredoc_ptr, delim))
	{
		if (!*g_heredoc_ptr)
		{
			if ((ret = line_get()) == 0)
				continue ;
			else if (ret == INPUT_INT)
			{
				ft_dstr_del((void **)&heredoc);
				return (NULL);
			}
			else if (ret == INPUT_EOF)
			{
				heredoc_eof(delim);
				g_heredoc_eof = true;
			}
		}
		line_add(heredoc);
	}
	g_heredoc_ptr += g_heredoc_eof ? 0 : ft_strlen(delim);
	return (heredoc);
}

int				get_all_heredocs(void)
{
	t_dstr	*heredoc;
	int		i;

	i = 0;
	if (g_parser.error || !g_parser.heredocs
	|| !(g_heredoc_ptr = ft_strchr(g_lexer.line, '\n')))
		return (0);
	g_heredoc_ptr++;
	while (g_parser.heredocs[i])
	{
		ft_dstr_add(g_parser.heredocs[i]->value, '\n');
		if (!(heredoc = get_heredoc(g_parser.heredocs[i]->value->str)))
			break ;
		g_lexer.i += heredoc->len
		+ (g_heredoc_eof ? 0 : g_parser.heredocs[i]->value->len);
		ft_dstr_del((void **)&g_parser.heredocs[i]->value);
		g_parser.heredocs[i]->value = heredoc;
		g_heredoc_eof = false;
		i++;
	}
	ft_memdel((void **)&g_parser.heredocs);
	g_heredoc_ptr = NULL;
	return (g_parser.error);
}
