/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdemirci <gdemirci@student.42kocaeli.  	+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 22:55:28 by gdemirci          #+#    #+#             */
/*   Updated: 2025/08/04 22:55:30 by gdemirci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

void	set_start_time(t_table *table)
{
	unsigned int	j;

	table->start_time = get_current_time();
	j = 0;
	pthread_mutex_lock(&table->meal_lock);
	while (j < table->nb_philos)
	{
		table->philos[j]->last_meal = table->start_time;
		j++;
	}
	pthread_mutex_unlock(&table->meal_lock);
	set_start(table);
}

void	print_action(t_philo *philo, char *action, bool state)
{
	size_t	time;

	pthread_mutex_lock(&philo->table->write_lock);
	if (check_stop(philo->table) && state == false)
	{
		pthread_mutex_unlock(&philo->table->write_lock);
		return ;
	}
	time = get_current_time() - philo->table->start_time;
	printf("%ld %d%s\n", time, philo->id, action);
	pthread_mutex_unlock(&philo->table->write_lock);
}

bool	is_dead(t_philo *philo)
{
	bool	dead;

	dead = false;
	pthread_mutex_lock(&philo->table->meal_lock);
	if ((long)(get_current_time() - philo->last_meal) >
		philo->table->time_to_die)
		dead = true;
	pthread_mutex_unlock(&philo->table->meal_lock);
	return (dead);
}

bool	is_all_ate(t_table *table)
{
	unsigned int	full_count;
	unsigned int	i;

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
	{
		set_stop(table);
		return (true);
	}
	return (false);
}
