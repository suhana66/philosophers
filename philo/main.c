/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susajid <susajid@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 18:02:25 by susajid           #+#    #+#             */
/*   Updated: 2024/01/26 12:47:12 by susajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	routine(t_philo *philo);
static bool	print(t_philo *philo, char *action);

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
	philo->last_meal = philo->sim->start_time;
	while (print(philo, NULL))
	{
		pthread_mutex_lock(fork1);
		if (!print(philo, TAKEN_FORK))
		{
			pthread_mutex_unlock(fork1);
			break ;
		}
		if (fork1 == fork2)
		{
			ft_usleep(philo->sim->time_to_die);
			pthread_mutex_unlock(fork1);
			break ;
		}
		pthread_mutex_lock(fork2);
		if (!print(philo, TAKEN_FORK) || !print(philo, EATING))
		{
			pthread_mutex_unlock(fork1);
			pthread_mutex_unlock(fork2);
			break ;
		}
		philo->last_meal = get_time();
		ft_usleep(philo->sim->time_to_eat);
		pthread_mutex_unlock(fork1);
		pthread_mutex_unlock(fork2);
		if (!print(philo, SLEEPING))
			break ;
		ft_usleep(philo->sim->time_to_sleep);
		if (!print(philo, THINKING))
			break ;
	}
	print(philo, DIED);
}

static bool	print(t_philo *philo, char *action)
{
	pthread_mutex_lock(&philo->sim->write);
	if (get_time() - philo->last_meal > philo->sim->time_to_die)
		philo->sim->quit = true;
	if (philo->sim->quit)
	{
		pthread_mutex_unlock(&philo->sim->write);
		return (false);
	}
	if (action)
		printf("%zu %zu %s\n",
			get_time() - philo->sim->start_time, philo->id, action);
	pthread_mutex_unlock(&philo->sim->write);
	return (true);
}
