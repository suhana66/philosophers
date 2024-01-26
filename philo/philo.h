/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susajid <susajid@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 15:54:45 by susajid           #+#    #+#             */
/*   Updated: 2024/01/26 12:39:06 by susajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>

# define USAGE_ERR "usage: ./philo <number_of_philo> <time_to_die> <time_to_eat> \
<time_to_sleep> [number_of_meals]"
# define INVALID_ARGS_ERR "an invalid argument was entered"
# define THREAD_CREATE_ERR "could not create thread"
# define THREAD_JOIN_ERR "could not join thread"
# define MALLOC_ERR "a memory allocation error occurred"
# define MUTEX_INIT_ERR "could not initialize mutex"
# define GET_TIME_ERR "could not access time"

# define TAKEN_FORK "has taken a fork"
# define THINKING "is thinking"
# define SLEEPING "is sleeping"
# define EATING "is eating"
# define DIED "died"

typedef struct s_simulation
{
	size_t			number_of_philo;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			number_of_meals;
	struct s_philo	*philos;
	pthread_mutex_t	write;
	bool			quit;
	size_t			start_time;
}	t_simulation;

typedef struct s_philo
{
	size_t				id;
	struct s_simulation	*sim;
	pthread_mutex_t		fork;
	pthread_t			thread;
	size_t				last_meal;
}	t_philo;

int		sim_init(t_simulation *sim, int argc, char **argv);
void	sim_destroy(t_simulation sim);

void	ft_perror(char *msg);
void	ft_usleep(size_t milliseconds);
size_t	get_time(void);

#endif /* PHILO_H */
