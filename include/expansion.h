/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 16:11:34 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/05 15:53:38 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORD_EXPANSION_H
# define WORD_EXPANSION_H

# include "shell.h"

int		expand(t_simple_cmd *command);
int		param_expand(t_dstr *str, int start, bool heredoc);
void	path_expand(t_token *token);
t_array	*get_matches(char *path);
bool	is_match(char *str, char *pat, char quote, bool is_first_char);

bool	quote_start(char *str, int i, char *quote_status);
bool	quote_stop(char *str, int i, char *quote_status);
void	remove_quotes(t_dstr *str);
void	remove_bslash(t_dstr *str);
void	sort_matches(char **array, int size);

#endif
