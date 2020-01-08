/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 16:11:34 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/08 18:21:07 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	WORD_EXPANSION_H
# define WORD_EXPANSION_H

#include "shell.h"

char	**expand(t_node *command, t_env *env);
int		param_expand(t_dstr *str, int start, t_env *env);

bool	is_quote_start(char *str, int i, char quote_status);
bool	is_quote_stop(char *str, int i, char quote_status);
void	remove_quotes(t_dstr *str);

#endif
