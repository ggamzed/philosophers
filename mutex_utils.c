/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdemirci <gdemirci@student.42kocaeli.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 21:09:33 by gdemirci          #+#    #+#             */
/*   Updated: 2025/07/30 21:09:38 by gdemirci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

void	set_stop(t_table *table)
{
	pthread_mutex_lock(&table->stop_lock);
	table->stop = 1;
	pthread_mutex_unlock(&table->stop_lock);
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

int	check_stop(t_table *table)
{
	int	finish;

	finish = 0;
	pthread_mutex_lock(&table->stop_lock);
	if (table->stop == 1)
		finish = 1;
	pthread_mutex_unlock(&table->stop_lock);
	return (finish);
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
