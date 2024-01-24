/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susajid <susajid@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 15:54:45 by susajid           #+#    #+#             */
/*   Updated: 2024/01/24 17:57:30 by susajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <pthread.h>

# define USAGE_ERR "usage: ./philo <number_of_philo> <time_to_die> <time_to_eat> \
<time_to_sleep> [number_of_meals]"
# define INVALID_ARGS_ERR "an invalid argument was entered"
# define THREAD_CREATE_ERR "could not create thread"
# define THREAD_JOIN_ERR "could not join thread"
# define MALLOC_ERR "a memory allocation error occurred"
# define MUTEX_INIT_ERR "could not initialize mutex"

typedef struct s_simulation
{
	unsigned int	number_of_philo;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	number_of_meals;
	struct s_philo	*philos;
	pthread_mutex_t	write;
	bool			quit;
}	t_simulation;

typedef struct s_philo
{
	unsigned int		id;
	struct s_simulation	sim;
	pthread_mutex_t		fork;
	pthread_t			thread;
}	t_philo;

void	ft_perror(char *msg);

#endif /* PHILO_H */
