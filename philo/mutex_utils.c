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

void	set_stop(t_table *table)
{
	pthread_mutex_lock(&table->stop_lock);
	table->stop = 1;
	pthread_mutex_unlock(&table->stop_lock);
}

void	set_start(t_table *table)
{
	pthread_mutex_lock(&table->start_lock);
	table->start = 1;
	pthread_mutex_unlock(&table->start_lock);
}

bool	check_start(t_table *table)
{
	bool	start;

	start = false;
	pthread_mutex_lock(&table->start_lock);
	start = table->start;
	pthread_mutex_unlock(&table->start_lock);
	return (start);
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
