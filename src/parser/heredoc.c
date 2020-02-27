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

/*
static int			remove_bslash_nl(t_dstr *str)
{
	int i;
	int	count;

	count = 0;
	i = str->len - 2;
	while (i >= 0 && str->str[i] == '\\')
	{
		count++;
		i--;
	}
	if (count % 2)
	{
		ft_dstr_remove(str, str->len - 2, 2);
		return (1);
	}
	return (0);
}

static bool			is_heredoc_end(t_dstr *heredoc, char *delim)
{
	int		i;
	char	*last_line;

	i = heredoc->len - 2;
	last_line = NULL;
	while (i >= 0)
	{
		if (heredoc->str[i] == '\n')
		{
			last_line = &heredoc->str[i + 1];
			break ;
		}
		i--;
	}
	if (i == -1)
		last_line = heredoc->str;
	if (last_line == NULL)
		return (false);
	return (ft_strequ(last_line, delim));
}
static char			*get_heredoc(char *delim)
{
	char		*str;
	t_dstr		*heredoc;
	char		*delim_cmp;

	delim_cmp = ft_strjoin(delim, "\n");
	heredoc = ft_dstr_new(32);
	while ((str = readline("> ")))
	{
		ft_dstr_insert(heredoc, heredoc->len, str, ft_strlen(str));
		remove_bslash_nl(heredoc);
		if (is_heredoc_end(heredoc, delim_cmp) || g_parse_error == SILENT_ABORT)
			break ;
	}
	if (g_shell_interactive)
		append_line_to_hist(heredoc->str);
	if (g_parse_error == SILENT_ABORT)
		ft_dstr_del((void **)&heredoc);
	else if (heredoc->len >= ft_strlen(delim_cmp))
		ft_dstr_remove(heredoc, heredoc->len - ft_strlen(delim_cmp)
						, ft_strlen(delim_cmp));
	free(delim_cmp);
	str = heredoc ? heredoc->str : NULL;
	free(heredoc);
	return (str);
}
*/

char		*get_heredoc(const char *delim)
{
	while (get_input(PSH) != EOF /* && !is_heredoc_end(delim)*/)
	{
		get_input(PSH);

	}
	return (0);
}

void		get_all_heredocs(void)
{
	char	*heredoc_str;
	int		i;

	i = 0;
	if (!g_parser.heredocs)
		return ;
	while (g_parser.heredocs[i])
	{
		heredoc_str = get_heredoc(g_parser.heredocs[i]->value->str);
		ft_dstr_del((void **)&g_parser.heredocs[i]->value);
		g_parser.heredocs[i]->value = ft_dstr_from_str(heredoc_str);
		free(heredoc_str);
		i++;
	}
	ft_memdel((void **)&g_parser.heredocs);
}
