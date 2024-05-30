/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susajid <susajid@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 10:20:47 by susajid           #+#    #+#             */
/*   Updated: 2024/05/08 13:21:03 by susajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define USAGE_ERR "use: ./philo <n_philo> <t_die> <t_eat> <t_sleep> [n_meal]\n"
# define INVALID_ARGS_ERR "an invalid argument was entered\n"
# define MUTEX_INIT_ERR "could not initialize mutex\n"
# define MALLOC_ERR "a memory allocation error occurred\n"
# define THREAD_CREATE_ERR "could not create thread\n"
# define THREAD_JOIN_ERR "could not join thread\n"
# define TIME_ERR "could not get time\n"

# define TAKEN_FORK "has taken a fork"
# define EATING "is eating"
# define SLEEPING "is sleeping"
# define THINKING "is thinking"
# define DEAD "died"

typedef struct s_simulation
{
	unsigned int	n_philo;
	unsigned int	t_die;
	unsigned int	t_eat;
	unsigned int	t_sleep;
	unsigned int	n_meal;
	int				if_limit;
	int				if_quit;
	size_t			start_time;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	write_lock;
	struct s_philo	*philos;
}					t_simulation;

typedef struct s_philo
{
	unsigned int		id;
	pthread_t			thread;
	struct s_simulation	*sim;
	int					eating;
	size_t				last_meal;
	unsigned int		meal_counter;
	pthread_mutex_t		fork;
}						t_philo;

int		sim_init(t_simulation *sim, int argc, char **argv);
int		str_to_natural(char *str, unsigned int *result);
int		philos_init(t_simulation *sim);
void	sim_destroy(t_simulation *sim);
void	sim_monitor(t_simulation *sim);
int		philos_finished(t_simulation *sim);
int		philos_dead(t_simulation *sim);
void	sim_quit(t_simulation *sim);

void	*routine(t_philo *philo);
int		eat(t_philo *philo, pthread_mutex_t	*fork1, pthread_mutex_t	*fork2);
void	pick_fork(t_philo *philo, pthread_mutex_t *fork);
void	print(t_philo *philo, char *action);

int		check_quit(t_simulation *sim);
int		check_last_meal(t_philo *philo);
void	do_sleep(size_t milliseconds, t_simulation *sim);
size_t	get_time(void);
void	ft_perror(char *msg);

#endif /* PHILO_H */