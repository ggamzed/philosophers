/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdemirci <gdemirci@student.42kocaeli.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 21:09:44 by gdemirci          #+#    #+#             */
/*   Updated: 2025/07/30 21:09:47 by gdemirci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <unistd.h>

static bool	is_all_ate(t_table *table)
{
	unsigned int	full_count;
	unsigned int i;

	if (table->must_eat == -1)
		return (false);
	full_count = 0;
	i = 0;
	while (i < table->nb_philos)
	{
		pthread_mutex_lock(&table->meal_lock);
		if (table->philos[i]->meals_eaten >= table->must_eat)
			++full_count;
		pthread_mutex_unlock(&table->meal_lock);
		i++;
	}
	if (full_count == table->nb_philos)
		return (true);
	return (false);
}

static void	*observer(void *ptr)
{
	t_philo			**philos;
	unsigned int	i;

	philos = (t_philo **)ptr;
	if (!philos || !philos[0] || !philos[0]->table)
		return (NULL);
	while (!check_stop(philos[0]->table))
	{
		i = 0;
		while (i < philos[0]->table->nb_philos)
		{
			if (is_dead(philos[i]))
			{
				set_stop(philos[0]->table);
				print_action(philos[i], " died", true);
				return (NULL);
			}
			i++;
		}
		if (is_all_ate(philos[0]->table))
		{
			set_stop(philos[0]->table);
			return (NULL);
		}
		usleep(100);
	}
	return (NULL);
}

static void	philo_routine(t_philo *philo)
{
	if (check_stop(philo->table))
		return ;
	pthread_mutex_lock(philo->left_fork);
	print_action(philo, " has taken a fork", false);
	if (check_stop(philo->table))
	{
		pthread_mutex_unlock(philo->left_fork);
		return ;
	}
	pthread_mutex_lock(philo->right_fork);
	print_action(philo, " has taken a fork", false);
	print_action(philo, " is eating", false);
	pthread_mutex_lock(&philo->table->meal_lock);
	philo->last_meal = get_current_time();
	pthread_mutex_unlock(&philo->table->meal_lock);
	ft_usleep(philo->table->time_to_eat);
	if (!check_stop(philo->table))
	{
		pthread_mutex_lock(&philo->table->meal_lock);
		philo->meals_eaten += 1;
		pthread_mutex_unlock(&philo->table->meal_lock);
	}
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	print_action(philo, " is sleeping", false);
	ft_usleep(philo->table->time_to_sleep);
}

static void	*philosophers(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	pthread_mutex_lock(&philo->table->meal_lock);
	philo->last_meal = get_current_time();
	pthread_mutex_unlock(&philo->table->meal_lock);
	if (philo->id % 2 == 0)
		ft_usleep(10);
	if (philo->table->nb_philos == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, " has taken a fork", false);
		ft_usleep(philo->table->time_to_die);
		print_action(philo, " died", true);
		pthread_mutex_unlock(philo->left_fork);
		return (NULL);
	}
	while (!check_stop(philo->table))
	{
		philo_routine(philo);
		if (!check_stop(philo->table))
			print_action(philo, " is thinking", false);
	}
	return (NULL);
}

void	ft_bzero(void *p, size_t size)
{
	while (size--)
		((unsigned char *)p)[size] = 0;
}

void	*let_the_meal_begin(t_table *table)
{
	pthread_t		observer_tid;
	unsigned int	i;

	table->start_time = get_current_time(); //+ (table->nb_philos * 2 * 10);
	i = 0;
	while (i < table->nb_philos)
	{
		if (pthread_create(&table->philos[i]->thread_id, NULL, philosophers,
				table->philos[i]) != 0)
		{
			error_message("Thread Create ERROR!\n");
			set_stop(table);
			break ;
		}
		i++;
	}
	ft_bzero(&observer_tid, sizeof(pthread_t));
	if (table->nb_philos > 1)
	{
		if (pthread_create(&observer_tid, NULL, &observer, table->philos) != 0)
			return (error_message("Thread Create ERROR!\n"));
	}
	if (join_threads(table, observer_tid, i))
		return (NULL);
	return (table);
}
