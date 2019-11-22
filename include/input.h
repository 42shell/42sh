/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:09:52 by fratajcz          #+#    #+#             */
/*   Updated: 2019/11/22 01:14:19 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# include "libft.h"
# include "_term.h"

typedef struct		s_input
{
	t_dstr			*line;
	int				pos;
}					t_input;

void	init_input(t_input *input);
int		read_line(t_term *term, t_input *input);

/*
** Utils
*/
int		echo_str(t_input *input, char *s);
int		echo_char(t_input *input, int c);
void	move_curs_right(t_input *input, t_term *term);
void	move_curs_left(t_input *input, t_term *term);

#endif
