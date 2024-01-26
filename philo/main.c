/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susajid <susajid@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 18:02:25 by susajid           #+#    #+#             */
/*   Updated: 2024/01/26 10:17:30 by susajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	routine(t_philo *philo);
static void	print(t_philo *philo, char *action);
static bool	is_dead(t_philo *philo);

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
		if (pthread_detach(sim.philos[i].thread))
			return (sim_destroy(sim), ft_perror(THREAD_DETACH_ERR), 4);
		i++;
	}
	while (!sim.quit)
		;
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
	while (!is_dead(philo))
	{
		pthread_mutex_lock(fork1);
		print(philo, TAKEN_FORK);
		if (fork1 == fork2)
		{
			ft_usleep(philo->sim->time_to_die + 1);
			break ;
		}
		pthread_mutex_lock(fork2);
		print(philo, TAKEN_FORK);
		print(philo, EATING);
		ft_usleep(philo->sim->time_to_eat);
		philo->last_meal = get_time();
		pthread_mutex_unlock(fork1);
		pthread_mutex_unlock(fork2);
		print(philo, SLEEPING);
		ft_usleep(philo->sim->time_to_sleep);
		print(philo, THINKING);
	}
	print(philo, DIED);
	philo->sim->quit = true;
}

static void	print(t_philo *philo, char *action)
{
	pthread_mutex_lock(&philo->sim->write);
	printf("%zu %zu %s\n",
		get_time() - philo->sim->start_time, philo->id, action);
	pthread_mutex_unlock(&philo->sim->write);
}

static bool	is_dead(t_philo *philo)
{
	return (get_time() - philo->last_meal > philo->sim->time_to_die
		&& !philo->sim->quit);
}
