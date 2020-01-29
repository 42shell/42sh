/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/07 20:03:18 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/29 15:32:19 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

extern int	g_parse_error;

static void			append_line_to_hist(t_list_head *hist_head, char *line)
{
	t_dstr	*hist;

	hist = (t_dstr *)hist_head->prev->data;
	ft_dstr_add(hist, '\n');
	ft_dstr_insert(hist, hist->len, line, ft_strlen(line));
	if (hist->str[hist->len - 1] == '\n')
	{
		hist->str[hist->len - 1] = '\0';
		hist->len--;
	}
}

static int			remove_bslash_nl(t_dstr *str)
{
	int i;
	int	count;

	count = 0;
	i = str->len - 2;
	while (str->str[i] == '\\')
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

static const char	*find_cmp(const char *s)
{
	int		i;

	i = ft_strlen(s) - 2;
	while (i >= 0)
	{
		if (s[i] == '\n')
			return (&s[i + 1]);
		i--;
	}
	if (i == -1)
		return (s);
	return (NULL);
}

static char			*get_heredoc(t_input *input, char *delim)
{
	char		*str;
	t_dstr		*heredoc;
	char		*delim_cmp;
	const char	*cmp;

	delim_cmp = ft_strjoin(delim, "\n");
	heredoc = ft_dstr_new("", 0, 32);
	input->first_line = false;
	while ((str = readline(input, "> ")))
	{
		if (input->interactive)
			append_line_to_hist(input->head, str);
		ft_dstr_insert(heredoc, heredoc->len, str, ft_strlen(str));
		if ((remove_bslash_nl(heredoc) == 0 && ((cmp = find_cmp(heredoc->str))
			&& ft_strequ(cmp, delim_cmp))) || g_parse_error == SILENT_ABORT)
			break ;
	}
	if (g_parse_error == SILENT_ABORT)
		ft_dstr_del((void **)&heredoc, NULL);
	else
		ft_dstr_remove(heredoc, heredoc->len - ft_strlen(cmp), ft_strlen(cmp));
	free(delim_cmp);
	str = heredoc ? heredoc->str : NULL;
	free(heredoc);
	return (str);
}

void				get_all_heredocs(t_input *input, t_node *heredoc_list)
{
	int		i;
	char	*heredoc_str;
	t_node	*heredoc;

	i = 0;
	while (i < heredoc_list->nb_children && g_parse_error != SILENT_ABORT)
	{
		heredoc = heredoc_list->child[i];
		heredoc_str = get_heredoc(input,
				node_token(heredoc->child[1])->value->str);
		free(node_token(heredoc->child[1])->value->str);
		node_token(heredoc->child[1])->value->str = heredoc_str;
		node_token(heredoc->child[1])->value->len = ft_strlen(heredoc_str);
		i++;
	}
	i = 0;
	while (i < heredoc_list->nb_children)
	{
		heredoc_list->child[i] = NULL;
		i++;
	}
	heredoc_list->nb_children = 0;
}
