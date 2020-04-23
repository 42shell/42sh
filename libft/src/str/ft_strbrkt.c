/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strbrkt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: exam <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 19:08:56 by exam              #+#    #+#             */
/*   Updated: 2019/06/21 19:36:29 by exam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_bracket_open(char c)
{
	return (c == '(' || c == '[' || c == '{');
}

static int	is_bracket_close(char c)
{
	return (c == ')' || c == ']' || c == '}');
}

static int	brackets_match(char a, char b)
{
	if (a == '(' && b != ')')
		return (0);
	if (a == '[' && b != ']')
		return (0);
	if (a == '{' && b != '}')
		return (0);
	return (1);
}

int			ft_strbrkt(char *str, int len)
{
	char	stack[len];
	int		top;
	int		i;

	top = 0;
	i = 0;
	ft_bzero(stack, len);
	while (str[i])
	{
		if (is_bracket_open(str[i]))
		{
			top++;
			stack[top] = str[i];
		}
		if (is_bracket_close(str[i]))
		{
			if (!(brackets_match(stack[top], str[i])))
				return (0);
			stack[top] = 0;
			top--;
		}
		i++;
	}
	return (top == 0);
}
