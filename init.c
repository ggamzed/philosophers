/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdemirci <gdemirci@student.42kocaeli.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 21:09:54 by gdemirci          #+#    #+#             */
/*   Updated: 2025/07/30 21:09:55 by gdemirci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>

static pthread_mutex_t	*init_forks(t_table *table)
{
	pthread_mutex_t	*forks;
	unsigned int	i;

	forks = malloc(sizeof(pthread_mutex_t) * table->nb_philos);
	if (!forks)
		return (error_message("Malloc ERROR!\n"));
	table->init_level = MALLOC_FORKS;
	i = 0;
	while (i < table->nb_philos)
	{
		if (pthread_mutex_init(&forks[i], 0) != 0)
			return (error_message("Mutex ERROR!\n"));
		table->init_level = INIT_FORKS;
		i++;
		table->forks_i = i;
	}
	return (forks);
}

static void	*init_table_mutexes(t_table *table)
{
	table->forks = init_forks(table);
	if (!table->forks)
		return (NULL);
	if (pthread_mutex_init(&table->write_lock, 0) != 0)
		return (error_message("Mutex ERROR!\n"));
	table->init_level = INIT_WRITE_MUTEX;
	if (pthread_mutex_init(&table->meal_lock, 0) != 0)
		return (error_message("Mutex ERROR!\n"));
	table->init_level = INIT_MEAL_MUTEX;
	if (pthread_mutex_init(&table->stop_lock, 0) != 0)
		return (error_message("Mutex ERROR!\n"));
	table->init_level = INIT_STOP_MUTEX;
	return (table);
}

static t_philo	**allocate_philos(t_table *table)
{
	t_philo			**philos;
	unsigned int	i;

	philos = malloc(sizeof(t_philo *) * table->nb_philos);
	if (!philos)
		return (error_message("Malloc ERROR!\n"));
	table->init_level = MALLOC_PHILOS;
	i = 0;
	while (i < table->nb_philos)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (error_message("Malloc ERROR!\n"));
		i++;
		table->philos_i = i;
	}
	return (philos);
}

static t_philo	**init_philo(t_table *table)
{
	t_philo			**philos;
	unsigned int	i;

	philos = allocate_philos(table);
	if (!philos)
		return (NULL);
	i = 0;
	while (i < table->nb_philos)
	{
		philos[i]->table = table;
		philos[i]->id = i + 1;
		philos[i]->last_meal = get_current_time();
		philos[i]->meals_eaten = 0;
		philos[i]->left_fork = &table->forks[i];
		if (i == 0)
			philos[i]->right_fork = &table->forks[table->nb_philos - 1];
		else
			philos[i]->right_fork = &table->forks[i - 1];
		i++;
	}
	return (philos);
}

t_table	*init_table(int argc, char **argv)
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	if (!table)
		return (error_message("Malloc ERROR!\n"));
	table->init_level = MALLOC_TABLE;
	table->forks_i = 0;
	table->philos_i = 0;
	table->stop = 0;
	table->nb_philos = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	table->must_eat = -1;
	if (argc == 6)
		table->must_eat = ft_atoi(argv[5]);
	if (!init_table_mutexes(table))
		return (NULL);
	table->philos = init_philo(table);
	if (!table->philos)
		return (NULL);
	return (table);
}
