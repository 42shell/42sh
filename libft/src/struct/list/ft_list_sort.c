/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_sort.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/09 15:12:24 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/16 15:42:00 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libft.h"

/*
** split a list into two equal parts
** each part goes respectively into *a and *b
** null terminates each split list
*/

void		list_split(t_list_head *head, t_list_head **a, t_list_head **b)
{
	t_list_head *fast;
	t_list_head *slow;

	fast = head;
	slow = head;
	if (head == NULL || head->next == NULL)
		return ;
	while (fast->next && fast->next->next)
	{
		fast = fast->next->next;
		slow = slow->next;
	}
	while (fast->next)
		fast = fast->next;
	*b = slow->next;
	*a = head;
	slow->next = NULL;
}

/*
** merge two lists in sorted order, return the result
*/

t_list_head	*list_merge(t_list_head *left, t_list_head *right,
						int (*cmp)(void *a, void *b))
{
	t_list_head			head;
	t_list_head			*tail;

	tail = &head;
	while (left && right)
	{
		if (cmp(left->data, right->data) <= 0)
		{
			tail->next = left;
			left = left->next;
		}
		else
		{
			tail->next = right;
			right = right->next;
		}
		tail = tail->next;
	}
	tail->next = left ? left : right;
	return (head.next);
}

void		list_sort_aux(t_list_head **head_ref, int (*cmp)(void *a, void *b))
{
	t_list_head	*a;
	t_list_head	*b;
	t_list_head	*head;

	head = *head_ref;
	if (head == NULL || head->next == NULL)
		return ;
	list_split(head, &a, &b);
	list_sort_aux(&a, cmp);
	list_sort_aux(&b, cmp);
	*head_ref = list_merge(a, b, cmp);
}

void		list_restore_links(t_list_head *head)
{
	t_list_head	*tmp;

	tmp = head;
	while (tmp->next)
	{
		tmp->next->prev = tmp;
		tmp = tmp->next;
	}
	tmp->next = head;
	head->prev = tmp;
}

/*
** WARNING: this sorts the head too, make sure that head->data != NULL and
** that your cmp function always make head->data inferior
*/

void		ft_list_sort(t_list_head **head, int (*cmp)(void *a, void *b))
{
	(*head)->prev->next = NULL;
	list_sort_aux(head, cmp);
	list_restore_links(*head);
}
