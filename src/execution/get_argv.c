/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_argv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/14 17:52:18 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char    **get_argv(t_command *command)
{
    t_token *word;
    char    **argv;
    int     count;

    count = 0;
    word = command->words;
    if (!word)
        return (NULL);
    while (word)
    {
        count++;
        word = word->next;
    }
    argv = ft_xmalloc(sizeof(char *) * (count + 1));
    word = command->words;
    count = 0;
    while (word)
    {
        argv[count++] = ft_strdup(word->value->str);
        word = word->next;
    }
    return (argv);
}