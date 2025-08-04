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

static void	check_args(int argc, char *argv[])
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
			exit(EXIT_FAILURE);
		}
		else if (i == 5 && (num < 1 || num > 2147483647))
		{
			error_message("Argument ERROR!\n");
			exit(EXIT_FAILURE);
		}
		else if (i != 1 && i != 5 && (num < 1 || num > 2147483647))
		{
			error_message("Argument ERROR!\n");
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

int	main(int argc, char *argv[])
{
	t_table	*table;

	if (argc < 5 || argc > 6)
	{
		error_message("Argument Count ERROR!\n");
		exit(EXIT_FAILURE);
	}
	table = NULL;
	check_args(argc, argv);
	table = init_table(argc, argv);
	if (!table)
		clean_all(table, 0);
	if (!let_the_meal_begin(table))
	{
		clean_all(table, 0);
		return (1);
	}
	clean_all(table, 1);
	return (0);
}
