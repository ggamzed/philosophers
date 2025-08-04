/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdemirci <gdemirci@student.42kocaeli.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 21:08:55 by gdemirci          #+#    #+#             */
/*   Updated: 2025/07/30 21:08:58 by gdemirci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <unistd.h>

void	*error_message(char *str)
{
	if (str)
		write(2, str, ft_strlen(str));
	return (NULL);
}

static void	free_philos(t_table *table)
{
	unsigned int	i;

	i = 0;
	if ((table->init_level >= MALLOC_PHILOS) && table->philos)
	{
		while (i < table->philos_i)
		{
			free(table->philos[i]);
			i++;
		}
		free(table->philos);
	}
}

static void	destroy_forks(t_table *table)
{
	unsigned int	i;

	i = 0;
	if (table->init_level >= INIT_FORKS)
	{
		while (i < table->forks_i)
		{
			pthread_mutex_destroy(&table->forks[i]);
			i++;
		}
	}
}

void	clean_all(t_table *table, unsigned int flag)
{
	if (!table)
		exit(EXIT_FAILURE);
	free_philos(table);
	if (table->init_level >= INIT_STOP_MUTEX)
		pthread_mutex_destroy(&table->stop_lock);
	if (table->init_level >= INIT_MEAL_MUTEX)
		pthread_mutex_destroy(&table->meal_lock);
	if (table->init_level >= INIT_WRITE_MUTEX)
		pthread_mutex_destroy(&table->write_lock);
	destroy_forks(table);
	if (table->init_level >= MALLOC_FORKS)
		free(table->forks);
	if (table->init_level >= MALLOC_TABLE)
		free(table);
	if (flag == 0)
		exit(EXIT_FAILURE);
}

int	join_threads(t_table *table, pthread_t obs_id, unsigned int cn)
{
	unsigned int	i;
	int				err_flag;

	err_flag = 0;
	if (table->nb_philos > 1)
	{
		if (pthread_join(obs_id, NULL) != 0)
		{
			error_message("Thread Join ERROR!\n");
			err_flag = 1;
		}
	}
	i = 0;
	while (i < cn)
	{
		if (pthread_join(table->philos[i]->thread_id, NULL) != 0)
		{
			error_message("Thread Join ERROR!\n");
			err_flag = 1;
		}
		i++;
	}
	return (err_flag);
}
