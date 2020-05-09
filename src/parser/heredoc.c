/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/07 20:03:18 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 18:34:56 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static char	*g_heredoc_ptr = NULL;
static char	g_heredoc_eof = false;

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

	while (!ft_strchr(g_heredoc_ptr, '\n'))
	{
		i = g_heredoc_ptr - g_lexer.line;
		if ((ret = g_shell.get_input(PSH, true)))
			return (ret);
		g_heredoc_ptr = &g_lexer.line[i];
	}
	return (0);
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
				ft_dstr_del(&heredoc);
				return (NULL);
			}
			else if (ret == INPUT_EOF)
			{
				parse_heredoc_eof(delim);
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
	t_token	*curr;
	t_token	*tmp;

	if (g_parser.status != NOERR || !(curr = g_parser.heredocs)
	|| !(g_heredoc_ptr = ft_strchr(g_lexer.line, '\n')))
		return (0);
	g_heredoc_ptr++;
	while (curr)
	{
		ft_dstr_add(curr->value, '\n');
		if (!(heredoc = get_heredoc(curr->value->str)))
			break ;
		g_lexer.i += heredoc->len + (g_heredoc_eof ? 0 : curr->value->len);
		ft_dstr_del(&curr->value);
		curr->value = heredoc;
		g_heredoc_eof = false;
		tmp = curr;
		curr = curr->next;
		tmp->next = NULL;
	}
	g_parser.heredocs = NULL;
	g_heredoc_ptr = NULL;
	return (g_parser.status);
}
