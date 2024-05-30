/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susajid <susajid@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 10:20:38 by susajid           #+#    #+#             */
/*   Updated: 2024/05/08 13:25:18 by susajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_simulation	sim;
	unsigned int	i;

	if (sim_init(&sim, argc - 1, &argv[1]))
		return (1);
	sim.start_time = get_time();
	i = 0;
	while (i < sim.n_philo)
	{
		sim.philos[i].last_meal = get_time();
		if (pthread_create(&sim.philos[i].thread, NULL,
				(void *(*)(void *))routine, &sim.philos[i]) || !++i)
			return (ft_perror(THREAD_CREATE_ERR), sim_destroy(&sim), 2);
	}
	while (!sim_quit(&sim))
		do_sleep(1, &sim);
	i = 0;
	while (i < sim.n_philo)
		if (pthread_join(sim.philos[i++].thread, NULL))
			return (ft_perror(THREAD_JOIN_ERR), sim_destroy(&sim), 3);
	return (sim_destroy(&sim), 0);
}

void	*routine(t_philo *philo)
{
	pthread_mutex_t	*fork1;
	pthread_mutex_t	*fork2;

	if (philo->id % 2)
	{
		fork1 = &philo->fork;
		fork2 = &philo->sim->philos[philo->id % philo->sim->n_philo].fork;
	}
	else
	{
		fork1 = &philo->sim->philos[philo->id % philo->sim->n_philo].fork;
		fork2 = &philo->fork;
	}
	while (!check_quit(philo->sim))
	{
		if (eat(philo, fork1, fork2))
			break ;
		print(philo, SLEEPING);
		do_sleep(philo->sim->t_sleep, philo->sim);
		print(philo, THINKING);
	}
	return (NULL);
}

int	eat(t_philo *philo, pthread_mutex_t	*fork1, pthread_mutex_t	*fork2)
{
	if (fork1 == fork2)
		return (print(philo, TAKEN_FORK), 1);
	pick_fork(philo, fork1);
	pick_fork(philo, fork2);
	pthread_mutex_lock(&philo->sim->meal_lock);
	philo->last_meal = get_time();
	philo->eating = 1;
	pthread_mutex_unlock(&philo->sim->meal_lock);
	print(philo, EATING);
	do_sleep(philo->sim->t_eat, philo->sim);
	pthread_mutex_lock(&philo->sim->meal_lock);
	philo->meal_counter++;
	philo->eating = 0;
	pthread_mutex_unlock(&philo->sim->meal_lock);
	pthread_mutex_unlock(fork1);
	pthread_mutex_unlock(fork2);
	return (0);
}

void	pick_fork(t_philo *philo, pthread_mutex_t *fork)
{
	pthread_mutex_lock(fork);
	print(philo, TAKEN_FORK);
}

void	print(t_philo *philo, char *action)
{
	pthread_mutex_lock(&philo->sim->write_lock);
	if (check_quit(philo->sim))
		return ((void)pthread_mutex_unlock(&philo->sim->write_lock));
	printf("%zu %d %s\n", get_time() - philo->sim->start_time,
		philo->id, action);
	pthread_mutex_unlock(&philo->sim->write_lock);
}
