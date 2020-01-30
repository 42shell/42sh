/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:08:08 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/16 15:39:06 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** Variables are static in case we reach the end of the buffer
** and the char is not terminated.
*/

int			addchar(t_input *input, uint8_t **bufp)
{
	static size_t	i = 0;
	static size_t	charlen = 0;
	size_t			offset;

	if (!charlen)
		charlen = ft_charlen(**bufp);
	while (**bufp && i < charlen)
		input->key[i++] = *(*bufp)++;
	if (i == charlen)
	{
		input->key[i] = 0;
		ft_dstr_insert(input->line, input->pos, (char *)input->key, charlen);
		if (input->interactive)
		{
			clearfromc(input->termp);
			offset = printstr(input->termp, &input->line->str[input->pos]);
			if (input->pos + charlen < input->line->len)
				movcto(input->termp, input->termp->cpos - offset + 1);
		}
		input->pos += charlen;
		charlen = 0;
		i = 0;
		return (0);
	}
	return (CONTINUE);
}

static int	process(t_input *input, uint8_t **bufp)
{
	static t_key_func *functable = NULL;

	if (!input->interactive)
		return (addchar(input, bufp));
	if (functable == NULL)
		functable = init_functable();
	if (**bufp != '\t')
		input->first_tab_press = true;
	if (input->esc)
		return (escape(input, bufp));
	if (ft_isprint(**bufp))
		return (addchar(input, bufp));
	if ((*input->key = *(*bufp)++) == 27)
		return (escape(input, bufp));
	if (*input->key == 127)
		return (backspace(input));
	if (*input->key < 32 && functable[*input->key])
		return (functable[*input->key](input));
	return (0);
}

static void	readline_init(t_input *input, const char *prompt)
{
	if (input->interactive)
	{
		draw_prompt(input, prompt);
		input->last_prompt = prompt;
	}
	if (input->line)
		ft_dstr_del((void **)&input->line, NULL);
	input->line = ft_dstr_new("", 0, 32);
	input->pos = 0;
	input->pos_min = 0;
}

static char	*readline_return(t_input *input)
{
	char *nl;

	if (input->line->str == NULL)
		return (NULL);
	if (input->multiline)
		input->line->str[0] = input->char_after_nl;
	if ((nl = ft_strchr(input->line->str, '\n')) && *(nl + 1) != '\0')
	{
		input->multiline = true;
		input->char_after_nl = *(nl + 1);
		*(nl + 1) = '\0';
	}
	else
		input->multiline = false;
	return (input->line->str);
}

/*
** We create a new line
** We read in a buffer of BUFSIZE (2bytes minimum).
** We process each char in the buffer then continue to read.
** It allows better perf on reading from a file,
** but also, in case of a esc sequence, it avoids problems we had
** in prev readline:
**	 if we have "\033[Dabcde", this was stored in an long, the esc sequence
**	 was not recognized by c == key, so it was not handled.
*/

char		*readline(t_input *input, const char *prompt)
{
	uint8_t		buf[BUFSIZE];
	uint8_t		*bufp;
	int			ret;

	if (input->multiline)
	{
		ft_dstr_remove(input->line, 1, ft_strlen(input->line->str));
		return (readline_return(input));
	}
	readline_init(input, prompt);
	while ((ret = read(STDIN_FILENO, buf, BUFSIZE - 1)) > 0)
	{
		bufp = buf;
		buf[ret] = 0;
		while (*bufp)
		{
			if ((ret = process(input, &bufp)) == CONTINUE)
				continue ;
			input->oldkey = *(uint64_t *)input->key;
			*(uint64_t *)input->key = 0;
			if (ret == EOL)
				return (readline_return(input));
		}
	}
	return ((ret == 0 && !*input->line->str) ? NULL : readline_return(input));
}
