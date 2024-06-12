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
	i = 0;
	sim.start_time = get_time();
	while (i < sim.n_philo)
	{
		sim.philos[i].last_meal = get_time();
		if (pthread_create(&sim.philos[i].thread, NULL,
				(void *(*)(void *))routine, &sim.philos[i]))
			return (sim_quit(&sim, i), ft_perror(THREAD_CREATE_ERR), 2);
		i++;
	}
	sim_monitor(&sim);
	sim_quit(&sim, sim.n_philo);
	return (0);
}

void	*routine(t_philo *philo)
{
	t_fork	*fork1;
	t_fork	*fork2;

	fork1 = &philo->fork;
	fork2 = &philo->sim->philos[philo->id % philo->sim->n_philo].fork;
	if (philo->id % 2 == 0)
	{
		fork1 = fork2;
		fork2 = &philo->fork;
		do_sleep(philo->sim->t_eat, philo->sim);
	}
	if (fork1 == fork2)
		return (print(philo, TAKEN_FORK, 1), NULL);
	while (!check_quit(philo->sim))
	{
		eat(philo, fork1, fork2);
		print(philo, SLEEPING, 1);
		do_sleep(philo->sim->t_sleep, philo->sim);
		print(philo, THINKING, 1);
	}
	return (NULL);
}

void	eat(t_philo *philo, t_fork *fork1, t_fork *fork2)
{
	change_fork_state(philo, fork1, 1);
	change_fork_state(philo, fork2, 1);
	pthread_mutex_lock(&philo->sim->mutex);
	philo->last_meal = get_time();
	philo->eating = 1;
	pthread_mutex_unlock(&philo->sim->mutex);
	print(philo, EATING, 1);
	do_sleep(philo->sim->t_eat, philo->sim);
	change_fork_state(philo, fork1, 0);
	change_fork_state(philo, fork2, 0);
	pthread_mutex_lock(&philo->sim->mutex);
	philo->eating = 0;
	philo->meal_counter++;
	if (philo->meal_counter == philo->sim->n_meal)
		philo->sim->satisfied++;
	pthread_mutex_unlock(&philo->sim->mutex);
}

void	change_fork_state(t_philo *philo, t_fork *fork, int if_take)
{
	if (!if_take)
	{
		pthread_mutex_lock(&fork->mutex);
		fork->available = 1;
		fork->owner_used = &philo->fork == fork;
		pthread_mutex_unlock(&fork->mutex);
		return ;
	}
	while (!check_quit(philo->sim))
	{
		pthread_mutex_lock(&fork->mutex);
		if (fork->available && (fork->owner_used == -1
				|| (fork->owner_used && &philo->fork != fork)
				|| (!fork->owner_used && &philo->fork == fork)))
		{
			fork->available = 0;
			pthread_mutex_unlock(&fork->mutex);
			print(philo, TAKEN_FORK, 1);
			return ;
		}
		pthread_mutex_unlock(&fork->mutex);
		usleep(100);
	}
}

void	print(t_philo *philo, char *action, int if_lock)
{
	if (if_lock)
		pthread_mutex_lock(&philo->sim->mutex);
	if (!philo->sim->if_quit)
	{
		printf("%zu %d %s\n", get_time() - philo->sim->start_time,
			philo->id, action);
	}
	if (if_lock)
		pthread_mutex_unlock(&philo->sim->mutex);
}
