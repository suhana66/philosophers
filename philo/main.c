/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susajid <susajid@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 10:20:38 by susajid           #+#    #+#             */
/*   Updated: 2024/02/11 16:24:22 by susajid          ###   ########.fr       */
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
		ft_usleep(1);
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

	fork1 = &philo->fork;
	fork2 = &philo->sim->philos[philo->id % philo->sim->n_philo].fork;
	if (philo->id % 2 == 0)
		ft_usleep(1);
	while (!check_quit(philo))
	{
		if (eat(philo, fork1, fork2))
			break ;
		print(philo, SLEEPING);
		ft_usleep(philo->sim->t_sleep);
		print(philo, THINKING);
	}
	return (NULL);
}

int	eat(t_philo *philo, pthread_mutex_t	*fork1, pthread_mutex_t	*fork2)
{
	pthread_mutex_lock(fork1);
	print(philo, TAKEN_FORK);
	if (fork1 == fork2)
		return (1);
	pthread_mutex_lock(fork2);
	print(philo, TAKEN_FORK);
	print(philo, EATING);
	pthread_mutex_lock(&philo->sim->meal_lock);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->sim->meal_lock);
	ft_usleep(philo->sim->t_eat);
	pthread_mutex_lock(&philo->sim->meal_lock);
	philo->meal_counter++;
	pthread_mutex_unlock(&philo->sim->meal_lock);
	pthread_mutex_unlock(fork1);
	pthread_mutex_unlock(fork2);
	return (0);
}

void	print(t_philo *philo, char *action)
{
	if (check_quit(philo))
		return ;
	pthread_mutex_lock(&philo->sim->write_lock);
	printf("%zu %d %s\n", get_time() - philo->sim->start_time,
		philo->id, action);
	pthread_mutex_unlock(&philo->sim->write_lock);
}

int	sim_quit(t_simulation *sim)
{
	unsigned int	i;
	unsigned int	satisfied_philos;
	int				if_dead;

	i = 0;
	satisfied_philos = 0;
	if_dead = 0;
	while (++i <= sim->n_philo)
	{
		if_dead = check_last_meal(&sim->philos[i - 1]);
		if (if_dead || (pthread_mutex_lock(&sim->meal_lock), 0))
			break ;
		if (sim->if_limit && sim->philos[i - 1].meal_counter >= sim->n_meal)
			satisfied_philos++;
		pthread_mutex_unlock(&sim->meal_lock);
	}
	if (if_dead)
		print(&sim->philos[i - 1], DEAD);
	if (if_dead || (sim->if_limit && satisfied_philos == sim->n_philo))
	{
		pthread_mutex_lock(&sim->dead_lock);
		sim->if_quit = 1;
		return (pthread_mutex_unlock(&sim->dead_lock), 1);
	}
	return (0);
}
