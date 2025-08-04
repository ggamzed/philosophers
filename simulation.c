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
#include <unistd.h>

static void	*observer(void *ptr)
{
	t_philo			**philos;
	unsigned int	i;

	philos = (t_philo **)ptr;
	if (!philos || !philos[0] || !philos[0]->table)
		return (NULL);
	while (!check_start(philos[0]->table))
		usleep(50);
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
			return (NULL);
	}
	return (NULL);
}

static void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, " has taken a fork", false);
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, " has taken a fork", false);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, " has taken a fork", false);
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, " has taken a fork", false);
	}
}

static void	philo_routine(t_philo *philo)
{
	take_forks(philo);
	print_action(philo, " is eating", false);
	ft_usleep(philo->table->time_to_eat);
	pthread_mutex_lock(&philo->table->meal_lock);
	philo->last_meal = get_current_time();
	philo->meals_eaten += 1;
	pthread_mutex_unlock(&philo->table->meal_lock);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	print_action(philo, " is sleeping", false);
	ft_usleep(philo->table->time_to_sleep);
}

static void	*philosophers(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	while (!check_start(philo->table))
		usleep(50);
	if (philo->id % 2 == 0)
		ft_usleep(philo->table->time_to_eat / 2);
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
		print_action(philo, " is thinking", false);
		ft_usleep(philo->table->custom_think);
	}
	return (NULL);
}

void	*let_the_meal_begin(t_table *table)
{
	pthread_t		observer_tid;
	unsigned int	i;

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
	set_start_time(table);
	if (join_threads(table, observer_tid, i))
		return (NULL);
	return (table);
}
