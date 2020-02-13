/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 16:11:34 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/13 19:45:40 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORD_EXPANSION_H
# define WORD_EXPANSION_H

# include "shell.h"

int		expand(t_node *command, t_env *env);
int		param_expand(t_dstr *str, int start, t_env *env, bool heredoc);
void	path_expand(t_node *pattern_node);

bool	quote_start(char *str, int i, char *quote_status);
bool	quote_stop(char *str, int i, char *quote_status);
void	remove_quotes(t_dstr *str);
void	remove_bslash(t_dstr *str);
void	sort_matches(t_node **array, int size);

#endif
