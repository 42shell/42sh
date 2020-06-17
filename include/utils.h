/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 12:28:09 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/29 02:08:29 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "shell.h"

char	*ft_strjoin_triple(char *s1, char *s2, char *s3);
void	free_arr(char **arr);
char	*append_filename(char *path, char *filename);
char	**split_path(char const *path);
int		get_opt(int argc, unsigned char *argv[]);
bool	is_valid_var_name(char *str);
char	*ft_mktemp(char *template);

#endif
