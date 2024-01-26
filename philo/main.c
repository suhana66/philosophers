/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susajid <susajid@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 18:02:25 by susajid           #+#    #+#             */
/*   Updated: 2024/01/29 16:15:46 by susajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	routine(t_philo *philo);
static int	eat(t_philo *philo, pthread_mutex_t *fork1, pthread_mutex_t *fork2);

int	main(int argc, char **argv)
{
	t_simulation	sim;
	size_t			i;

	if (sim_init(&sim, argc - 1, &argv[1]))
		return (1);
	i = 0;
	sim.start_time = get_time();
	while (i < sim.number_of_philo)
	{
		if (pthread_create(&sim.philos[i].thread, NULL,
				(void *(*)(void *))routine, &sim.philos[i]))
			return (sim_destroy(sim), ft_perror(THREAD_CREATE_ERR), 3);
		i++;
	}
	i = 0;
	while (i < sim.number_of_philo)
	{
		if (pthread_join(sim.philos[i].thread, NULL))
			return (sim_destroy(sim), ft_perror(THREAD_JOIN_ERR), 4);
		i++;
	}
	return (sim_destroy(sim), 0);
}

static void	routine(t_philo *philo)
{
	pthread_mutex_t	*fork1;
	pthread_mutex_t	*fork2;

	fork1 = &philo->sim->philos[0].fork;
	fork2 = &philo->fork;
	if (philo->id != philo->sim->number_of_philo)
	{
		fork1 = &philo->fork;
		fork2 = &philo->sim->philos[philo->id].fork;
	}
	philo->last_meal = philo->sim->start_time;
	while (print(philo, NULL))
	{
		if (eat(philo, fork1, fork2))
			return ;
		if (!print(philo, SLEEPING))
			return ;
		ft_usleep(philo->sim->time_to_sleep);
		if (!print(philo, THINKING))
			return ;
	}
}

static int	eat(t_philo *philo, pthread_mutex_t *fork1, pthread_mutex_t *fork2)
{
	pthread_mutex_lock(fork1);
	if (!print(philo, TAKEN_FORK) || fork1 == fork2)
		return (pthread_mutex_unlock(fork1), 1);
	pthread_mutex_lock(fork2);
	if (!print(philo, TAKEN_FORK) || !print(philo, EATING))
		return (pthread_mutex_unlock(fork1), pthread_mutex_unlock(fork2), 3);
	philo->last_meal = get_time();
	ft_usleep(philo->sim->time_to_eat);
	pthread_mutex_unlock(fork1);
	pthread_mutex_unlock(fork2);
	return (0);
}
