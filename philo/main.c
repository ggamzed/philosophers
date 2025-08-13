/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdemirci <gdemirci@student.42kocaeli.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 21:09:26 by gdemirci          #+#    #+#             */
/*   Updated: 2025/07/30 21:09:28 by gdemirci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>

static int	check_args(int argc, char *argv[])
{
	int		i;
	long	num;

	i = 1;
	while (i < argc)
	{
		num = ft_atoi(argv[i]);
		if (i == 1 && (num < 1 || num > PHILO_MAX_COUNT))
		{
			error_message("Argument ERROR\n");
			return (1);
		}
		else if (i == 5 && (num < 1 || num > 2147483647))
		{
			error_message("Argument ERROR!\n");
			return (1);
		}
		else if (i != 1 && i != 5 && (num < 1 || num > 2147483647))
		{
			error_message("Argument ERROR!\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	t_table	*table;
	t_table	*check_null;

	if (argc < 5 || argc > 6)
	{
		error_message("Argument Count ERROR!\n");
		return (1);
	}
	table = NULL;
	check_null = NULL;
	if (check_args(argc, argv))
		return (1);
	check_null = init_table(argc, argv, &table);
	if (!check_null)
		return (clean_all(table, 1));
	if (!let_the_meal_begin(table))
		return (clean_all(table, 1));
	return (clean_all(table, 0));
}
