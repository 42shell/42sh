/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomplete.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 12:28:09 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 17:19:49 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOCOMPLETE_H
# define AUTOCOMPLETE_H

# include "shell.h"

int					*g_comp_list_count;

t_list_head			*get_autocomplete_list(char *context, size_t len,
					int *count);
t_list_head			*comp_get_command_list(char *partial);
t_list_head			*get_file_list(char *partial, int flags);

bool				is_cd(char *str, int i);
bool				is_exec(char *path);
bool				is_dir(char *path);

#endif
