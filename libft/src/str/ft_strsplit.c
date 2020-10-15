/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/06 01:50:02 by fratajcz          #+#    #+#             */
/*   Updated: 2019/04/06 18:17:55 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static int	get_nb_words(char const *s, char c)
{
	int started_word;
	int word_count;

	started_word = 0;
	word_count = 0;
	while (*s)
	{
		if (!started_word && *s != c)
			started_word = 1;
		else if (started_word && *s == c)
		{
			started_word = 0;
			word_count++;
		}
		s++;
	}
	if (started_word)
		word_count++;
	return (word_count);
}

static int	get_wlen(char const *s, char c)
{
	int i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

char		**ft_strsplit(char const *s, char c)
{
	char	**tab;
	int		i;
	int		nb_words;
	int		wlen;

	nb_words = get_nb_words(s, c);
	if (!(tab = malloc(sizeof(char *) * (nb_words + 1))))
		return (NULL);
	i = 0;
	while (i < nb_words)
	{
		while (*s == c)
			s++;
		wlen = get_wlen(s, c);
		if (!(tab[i] = ft_strndup(s, wlen)))
			return (NULL);
		i++;
		s += wlen;
	}
	tab[i] = 0;
	return (tab);
}
