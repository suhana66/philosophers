/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susajid <susajid@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 18:02:25 by susajid           #+#    #+#             */
/*   Updated: 2024/01/25 18:24:12 by susajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	routine(t_philo *philo);
static void	print(t_philo *philo, char *action);

int	main(int argc, char **argv)
{
	t_simulation	sim;
	int				i;

	if (sim_init(&sim, argc - 1, &argv[1]))
		return (1);
	i = -1;
	sim.start_time = get_time();
	if (!sim.start_time)
		return (sim_destroy(sim), ft_perror(GET_TIME_ERR), 2);
	while (++i < (int)sim.number_of_philo)
	{
		if (pthread_create(&sim.philos[i].thread, NULL,
				(void *(*)(void *))routine, &sim.philos[i]))
			return (sim_destroy(sim), ft_perror(THREAD_CREATE_ERR), 3);
		if (pthread_detach(sim.philos[i].thread))
			return (sim_destroy(sim), ft_perror(THREAD_DETACH_ERR), 4);
	}
	while (!sim.quit)
		;
	return (sim_destroy(sim), 0);
}

static void	routine(t_philo *philo)
{
	pthread_mutex_t *fork1;
	pthread_mutex_t	*fork2;

	if (philo->id == philo->sim->number_of_philo)
	{
		fork1 = &philo->sim->philos[0].fork;
		fork2 = &philo->fork;
	}
	else
	{
		fork1 = &philo->fork;
		fork2 = &philo->sim->philos[philo->id].fork;
	}
	pthread_mutex_lock(fork1);
	print(philo, TAKEN_FORK);
	if (fork1 == fork2)
	{
		philo->sim->quit = true;
		return ;
	}
	pthread_mutex_lock(fork2);
	print(philo, TAKEN_FORK);
	print(philo, EATING);
	usleep(philo->sim->time_to_eat);
	pthread_mutex_unlock(fork1);
	pthread_mutex_unlock(fork2);
	print(philo, SLEEPING);
	usleep(philo->sim->time_to_sleep);
	print(philo, THINKING);
	print(philo, DIED);
	philo->sim->quit = true;
}

static void	print(t_philo *philo, char *action)
{
	pthread_mutex_lock(&philo->sim->write);
	printf("%llu %d %s\n", get_time() - philo->sim->start_time, philo->id, action);
	pthread_mutex_unlock(&philo->sim->write);
}
