/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdemirci <gdemirci@student.42kocaeli.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 21:10:02 by gdemirci          #+#    #+#             */
/*   Updated: 2025/07/30 21:10:04 by gdemirci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdbool.h>
# include <pthread.h>

# define PHILO_MAX_COUNT 200

typedef struct s_philo	t_philo;

typedef enum e_init_level
{
	MALLOC_TABLE = 1,
	MALLOC_FORKS,
	INIT_FORKS,
	INIT_WRITE_MUTEX,
	INIT_MEAL_MUTEX,
	INIT_STOP_MUTEX,
	INIT_START_MUTEX,
	MALLOC_PHILOS,
}	t_init_level;

typedef struct s_table
{
	pthread_mutex_t	*forks;
	t_philo			**philos;
	time_t			start_time;
	time_t			custom_think;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	stop_lock;
	unsigned int	nb_philos;
	int				must_eat;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	t_init_level	init_level;
	unsigned int	forks_i;
	unsigned int	philos_i;
	int				stop;
	int				start;
	pthread_mutex_t	start_lock;
}	t_table;

typedef struct s_philo
{
	int				id;
	pthread_t		thread_id;
	int				meals_eaten;
	time_t			last_meal;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_table			*table;
}	t_philo;

void			*error_message(char *str);
void			clean_all(t_table *table, unsigned int flag);
int				join_threads(t_table *table, pthread_t obs_id, unsigned int cn);
t_table			*init_table(int argc, char **argv);
void			set_stop(t_table *table);
void			set_start(t_table *table);
bool			check_start(t_table *table);
int				check_stop(t_table *table);
void			set_start_time(t_table *table);
void			print_action(t_philo *philo, char *action, bool state);
bool			is_dead(t_philo *philo);
bool			is_all_ate(t_table *table);
void			ft_bzero(void *p, size_t size);
void			*let_the_meal_begin(t_table *table);
time_t			get_current_time(void);
void			ft_usleep(size_t mls);
unsigned int	ft_strlen(const char *s);
long			ft_atoi(const char *str);

#endif
