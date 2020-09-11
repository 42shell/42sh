/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/11 16:01:48 by fratajcz          #+#    #+#             */
/*   Updated: 2020/09/11 16:01:48 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

typedef int(*t_test_func)(char **argv);

bool		is_number(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

void		init_test_hash(t_ht **operations)
{
	*operations = ht_new(32, free);
	ht_put(*operations, "-b", test_b);
	ht_put(*operations, "-c", test_c);
	ht_put(*operations, "-d", test_d);
	ht_put(*operations, "-e", test_e);
	ht_put(*operations, "-f", test_f);
	ht_put(*operations, "-g", test_g);
	ht_put(*operations, "-L", test_l);
	ht_put(*operations, "-p", test_p);
	ht_put(*operations, "-r", test_r);
	ht_put(*operations, "-S", test_sock);
	ht_put(*operations, "-s", test_size);
	ht_put(*operations, "-t", test_t);
	ht_put(*operations, "-u", test_u);
	ht_put(*operations, "-w", test_w);
	ht_put(*operations, "-x", test_x);
	ht_put(*operations, "-z", test_z);
	ht_put(*operations, "=", test_equal);
	ht_put(*operations, "!=", test_unequal);
	ht_put(*operations, "-eq", test_eq);
	ht_put(*operations, "-ne", test_ne);
	ht_put(*operations, "-gt", test_gt);
	ht_put(*operations, "-ge", test_ge);
	ht_put(*operations, "-lt", test_lt);
	ht_put(*operations, "-le", test_le);
}

static int	builtin_test_part2(char **argv, int argc, t_ht *operations)
{
	if (argc == 3)
	{
		if (!ht_get(operations, argv[1]))
		{
			ft_dprintf(2, "42sh: test: %s: unary operator expected\n", argv[1]);
			return (2);
		}
		return (((t_test_func)(ht_get(operations, argv[1])))(argv));
	}
	else if (argc == 4)
	{
		if (!ht_get(operations, argv[2]))
		{
			ft_dprintf(2, "42sh: test: %s: binary operator expected\n",
					argv[2]);
			return (2);
		}
		return (((t_test_func)(ht_get(operations, argv[2])))(argv));
	}
	else
	{
		ft_dprintf(2, "42sh: test: too many arguments\n", argv[2]);
		return (2);
	}
}

int			builtin_test(char **argv, __attribute__((unused)) t_array *env)
{
	static t_ht	*operations;
	int			argc;
	int			ret;
	bool		negate;

	if (operations == NULL)
		init_test_hash(&operations);
	argc = 0;
	while (argv[argc])
		argc++;
	if (argc == 1)
		return (1);
	if (argc == 2)
		return (argv[1][0] == '\0');
	if ((negate = (argv[1][0] == '!' && argv[1][1] == '\0')))
	{
		argv++;
		argc--;
	}
	if (argc == 2)
		ret = (argv[1][0] == '\0');
	else
		ret = builtin_test_part2(argv, argc, operations);
	return ((ret == 1 || ret == 0) ? (ret != negate) : ret);
}
