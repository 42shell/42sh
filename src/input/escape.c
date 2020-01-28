/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   escape.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/13 16:42:27 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/13 16:44:10 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static uint64_t	get_csi_code(t_input *input, uint8_t **bufp)
{
	static int	i = 1;
	uint64_t	code;

	while (**bufp)
	{
		input->key[i] = *(*bufp)++;
		if (i != 1 && '@' <= input->key[i])
		{
			code = *(uint64_t *)input->key;
			input->esc = false;
			i = 1;
			return (code);
		}
		i += i < 7 ? 1 : 0;
	}
	return (0);
}

int				escape(t_input *input, uint8_t **bufp)
{
	uint64_t	code;

	input->esc = true;
	if (!(code = get_csi_code(input, bufp)))
		return (CONTINUE);
	else if (code == input->keys[K_UP])
		return (history_up(input));
	else if (code == input->keys[K_DOWN])
		return (history_down(input));
	else if (code == input->keys[K_LEFT])
		return (move_left(input));
	else if (code == input->keys[K_RIGHT])
		return (move_right(input));
	else if (code == input->keys[K_HOME])
		return (move_home(input));
	else if (code == input->keys[K_END])
		return (move_end(input));
	else if (code == input->keys[K_DEL])
		return (delete(input));
	return (0);
}
